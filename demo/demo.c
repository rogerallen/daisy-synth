//------------------------------------------------------------------------------
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "demo.h"
#include "sokol_audio.h"
#include "sokol_gl.h"
#include "sokol_imgui.h"
#include <assert.h>

static struct {
    sg_pass_action pass_action;
    sgl_pipeline pip_3d;
    float cur_amplitude;
} state;

static void init(void)
{
    sg_setup(&(sg_desc){
        .context = sapp_sgcontext(),
        .logger.func = slog_func,
    });
    simgui_setup(&(simgui_desc_t){0});

    // init sokol-audio with default params
    saudio_setup(&(saudio_desc){
        .stream_cb = stream_cb,
        .logger.func = slog_func,
    });
    daisy_init(saudio_sample_rate());

    // setup sokol-gl
    sgl_setup(&(sgl_desc_t){
        .logger.func = slog_func,
    });

    // create a pipeline object for 3d rendering, with less-equal
    // depth-test and cull-face enabled, note that we don't provide
    // a shader, vertex-layout, pixel formats and sample count here,
    // these are all filled in by sokol-gl
    state.pip_3d = sgl_make_pipeline(&(sg_pipeline_desc){
        .cull_mode = SG_CULLMODE_BACK,
        .depth =
            {
                .write_enabled = true,
                .compare = SG_COMPAREFUNC_LESS_EQUAL,
            },
    });

    // initial clear color
    state.pass_action =
        (sg_pass_action){.colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                                       .clear_value = {0.0f, 0.5f, 1.0f, 1.0}}};

    // other state
    state.cur_amplitude = 0.05;
}

// draw a proper keyboard
// https://bootcamp.uxdesign.cc/drawing-a-flat-piano-keyboard-in-illustrator-de07c74a64c6?gi=866815891348
// a single octave is divided into seven diatonic steps and twelve chromatic
// steps. our range is 48/C3 to 76/E5.  But we will calculate things based on 3
// octaves [48,84) or 21 diatonic steps and 36 chromatic steps
// FIXME depth should work & we should not need draw_white_keys set
static void draw_key(int pitch, bool active, bool draw_white_keys)
{
    // x ranges from -1 to 1
    float margin_x = 0.1;
    float start_x = -1 + margin_x;
    float end_x = 1 - margin_x;
    // 7*2+3 white keys fit between start_x & end_x
    float white_dx = (end_x - start_x) / (7 * 2 + 3);
    // 12 black keys fit within 7 white keys
    float black_dx = 7 * white_dx / 12;
    // 48                     60                     72                     84
    // C                      C                      C                      C
    // w,b,w,b,w,w,b,w,b,w,b, w,b,w,b,w,w,b,w,b,w,b, w,b,w,b,w,w,b,w,b,w,b,
    int white_key_idx_ary[] = {
        0,  -1,     1,      -1,    2,      3,     -1,     4,     -1,
        5,  -1,     6,      7 + 0, -1,     7 + 1, -1,     7 + 2, 7 + 3,
        -1, 7 + 4,  -1,     7 + 5, -1,     7 + 6, 14 + 0, -1,    14 + 1,
        -1, 14 + 2, 14 + 3, -1,    14 + 4, -1,    14 + 5, -1,    14 + 6};
    int black_key_idx_ary[] = {
        -1, 1,      -1, 3,      -1, -1, 6,      -1, 8,      -1, 10,      -1,
        -1, 12 + 1, -1, 12 + 3, -1, -1, 12 + 6, -1, 12 + 8, -1, 12 + 10, -1,
        -1, 24 + 1, -1, 24 + 3, -1, -1, 24 + 6, -1, 24 + 8, -1, 24 + 10, -1};
    int p = pitch - 48;
    assert(sizeof(white_key_idx_ary) == sizeof(black_key_idx_ary));
    assert(p < sizeof(white_key_idx_ary));
    assert(p >= 0);
    int white_key_idx = white_key_idx_ary[p];
    int black_key_idx = black_key_idx_ary[p];
    // one should be -1, the other should be 0 greater
    assert(((white_key_idx >= 0) && (black_key_idx < 0)) ||
           ((white_key_idx < 0) && (black_key_idx >= 0)));
    if (white_key_idx >= 0 && draw_white_keys) {
        float x = start_x + white_key_idx * white_dx;
        float dx = white_dx * 0.95;
        int top_color = active ? 200 : 150;
        int bot_color = active ? 255 : 200;
        sgl_begin_quads();
        sgl_v3f_c3b(x, -0.0f, 0.9f, top_color, top_color, top_color);
        sgl_v3f_c3b(x, 0.5f, 0.9f, bot_color, bot_color, bot_color);
        sgl_v3f_c3b(x + dx, 0.5f, 0.9f, bot_color, bot_color, bot_color);
        sgl_v3f_c3b(x + dx, -0.0f, 0.9f, top_color, top_color, top_color);
        sgl_end();
    }
    else if (black_key_idx >= 0 && !draw_white_keys) {
        float x = start_x + black_key_idx * black_dx;
        float dx = black_dx * 1.0;
        int top_color = active ? 120 : 40;
        int bot_color = active ? 150 : 50;
        sgl_begin_quads();
        sgl_v3f_c3b(x, 0.2f, 0.5f, top_color, top_color, top_color);
        sgl_v3f_c3b(x, 0.5f, 0.5f, bot_color, bot_color, bot_color);
        sgl_v3f_c3b(x + dx, 0.5f, 0.5f, bot_color, bot_color, bot_color);
        sgl_v3f_c3b(x + dx, 0.2f, 0.5f, top_color, top_color, top_color);
        sgl_end();
    }
}

static void frame(void)
{
    simgui_new_frame(&(simgui_frame_desc_t){
        .width = sapp_width(),
        .height = sapp_height(),
        .delta_time = sapp_frame_duration(),
        .dpi_scale = sapp_dpi_scale(),
    });

    /*=== UI CODE STARTS HERE ===*/
    igSetNextWindowPos((ImVec2){10, 10}, ImGuiCond_Once, (ImVec2){0, 0});
    igSetNextWindowSize((ImVec2){400, 100}, ImGuiCond_Once);
    igBegin("Controls", 0, ImGuiWindowFlags_None);
    igText("Press a key to make a Sound.");
    // igColorEdit3("Background",
    //              &state.pass_action.colors[0].clear_value.r,
    //              ImGuiColorEditFlags_None);
    igLabelText("Pitch", "%d", get_pitch());
    igSliderFloat("Amplitude", &state.cur_amplitude, 0.0, 1.0, "%.3f",
                  ImGuiSliderFlags_None);
    igEnd();
    /*=== UI CODE ENDS HERE ===*/

    // sgl code start here
    sgl_defaults();
    sgl_viewport(0, 0, sapp_width(), sapp_height(), true);
#if 0
    // FIXME I cannot get depth test working...
    sgl_push_pipeline();
    sgl_load_pipeline(state.pip_3d);
    sgl_matrix_mode_projection();
    sgl_ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    sgl_matrix_mode_modelview();
#endif
    for (int p = 3 * 12 + 12; p <= 3 * 12 + 40; p++) {
        draw_key(p, p == get_pitch(), true);
    }
    // FIXME draw black keys second since depth test is failing
    for (int p = 3 * 12 + 12; p <= 3 * 12 + 40; p++) {
        draw_key(p, p == get_pitch(), false);
    }
#if 0
    sgl_pop_pipeline();
#endif
    int err = sgl_error();
    if (err != SGL_NO_ERROR) {
        printf("SGL_ERROR = %d\n", err);
    }

    // default pass
    sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
    sgl_draw();
    simgui_render();
    sg_end_pass();
    sg_commit();
}

static void cleanup(void)
{
    simgui_shutdown();
    sg_shutdown();
    saudio_shutdown();
}

// return midi pitch for keyboard events.  return -1 for no match
// range is 48/C3 to 76/E5
static int key_code_to_pitch(sapp_keycode kc)
{
    int pitch = -1;
    switch (kc) {
    case SAPP_KEYCODE_Z:
        pitch = 48; // C3
        break;
    case SAPP_KEYCODE_S:
        pitch = 49;
        break;
    case SAPP_KEYCODE_X:
        pitch = 50;
        break;
    case SAPP_KEYCODE_D:
        pitch = 51;
        break;
    case SAPP_KEYCODE_C:
        pitch = 52;
        break;
    case SAPP_KEYCODE_V:
        pitch = 53;
        break;
    case SAPP_KEYCODE_G:
        pitch = 54;
        break;
    case SAPP_KEYCODE_B:
        pitch = 55;
        break;
    case SAPP_KEYCODE_H:
        pitch = 56;
        break;
    case SAPP_KEYCODE_N:
        pitch = 57;
        break;
    case SAPP_KEYCODE_J:
        pitch = 58;
        break;
    case SAPP_KEYCODE_M:
        pitch = 59;
        break;
    case SAPP_KEYCODE_COMMA:
        pitch = 60; // C4 = Middle C
        break;
    case SAPP_KEYCODE_L:
        pitch = 61;
        break;
    case SAPP_KEYCODE_PERIOD:
        pitch = 62;
        break;

        // upper keyboard
    case SAPP_KEYCODE_Q:
        pitch = 60; // C4 = Middle C
        break;
    case SAPP_KEYCODE_2:
        pitch = 61;
        break;
    case SAPP_KEYCODE_W:
        pitch = 62;
        break;
    case SAPP_KEYCODE_3:
        pitch = 63;
        break;
    case SAPP_KEYCODE_E:
        pitch = 64;
        break;
    case SAPP_KEYCODE_R:
        pitch = 65;
        break;
    case SAPP_KEYCODE_5:
        pitch = 66;
        break;
    case SAPP_KEYCODE_T:
        pitch = 67;
        break;
    case SAPP_KEYCODE_6:
        pitch = 68;
        break;
    case SAPP_KEYCODE_Y:
        pitch = 69;
        break;
    case SAPP_KEYCODE_7:
        pitch = 70;
        break;
    case SAPP_KEYCODE_U:
        pitch = 71;
        break;
    case SAPP_KEYCODE_I:
        pitch = 72; // C5
        break;
    case SAPP_KEYCODE_9:
        pitch = 73;
        break;
    case SAPP_KEYCODE_O:
        pitch = 74;
        break;
    case SAPP_KEYCODE_0:
        pitch = 75;
        break;
    case SAPP_KEYCODE_P:
        pitch = 76; // E5
        break;
    }
    return pitch;
}

static void event(const sapp_event *ev)
{
    bool handled = simgui_handle_event(ev);
    if (true) { // !handled) {
        if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
            if (ev->key_code == SAPP_KEYCODE_ESCAPE) {
                sapp_quit();
            }
            else {
                int pitch = key_code_to_pitch(ev->key_code);
                if (pitch > 0) {
                    // printf("Note On  %d\n", pitch);
                    note_on(pitch, state.cur_amplitude);
                }
            }
        }
        else if (ev->type == SAPP_EVENTTYPE_KEY_UP) {
            int pitch = key_code_to_pitch(ev->key_code);
            if (pitch > 0) {
                // printf("Note Off %d\n", pitch);
                note_off(pitch);
            }
        }
    }
}

sapp_desc sokol_main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .window_title = "daisy-synth",
        .width = 800,
        .height = 800,
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}
