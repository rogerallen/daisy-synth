//------------------------------------------------------------------------------
//  Simple C99 cimgui+sokol starter project for Win32, Linux and macOS.
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
}

static void draw_triangle(void)
{
    sgl_defaults();
    sgl_begin_triangles();
    sgl_v2f_c3b(0.0f, 0.5f, 255, 0, 0);
    sgl_v2f_c3b(-0.5f, -0.5f, 0, 0, 255);
    sgl_v2f_c3b(0.5f, -0.5f, 0, 255, 0);
    sgl_end();
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
    igColorEdit3("Background", &state.pass_action.colors[0].clear_value.r,
                 ImGuiColorEditFlags_None);
    igEnd();
    /*=== UI CODE ENDS HERE ===*/

    // sgl code start here
    // compute viewport rectangles so that the views are horizontally
    // centered and keep a 1:1 aspect ratio
    const int dw = sapp_width();
    const int dh = sapp_height();
    const int ww = dh / 2; // not a bug
    const int hh = dh / 2;
    const int x0 = dw / 2 - hh;
    const int x1 = dw / 2;
    const int y0 = 0;
    const int y1 = dh / 2;
    // all sokol-gl functions except sgl_draw() can be called anywhere in the
    // frame
    sgl_viewport(x0, y0, ww, hh, true);
    draw_triangle();

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
static int key_code_to_pitch(sapp_keycode kc)
{
    int pitch = -1;
    switch (kc) {
    case SAPP_KEYCODE_Z:
        pitch = 12;
        break;
    case SAPP_KEYCODE_S:
        pitch = 13;
        break;
    case SAPP_KEYCODE_X:
        pitch = 14;
        break;
    case SAPP_KEYCODE_D:
        pitch = 15;
        break;
    case SAPP_KEYCODE_C:
        pitch = 16;
        break;
    case SAPP_KEYCODE_V:
        pitch = 17;
        break;
    case SAPP_KEYCODE_G:
        pitch = 18;
        break;
    case SAPP_KEYCODE_B:
        pitch = 19;
        break;
    case SAPP_KEYCODE_H:
        pitch = 20;
        break;
    case SAPP_KEYCODE_N:
        pitch = 21;
        break;
    case SAPP_KEYCODE_J:
        pitch = 22;
        break;
    case SAPP_KEYCODE_M:
        pitch = 23;
        break;
    case SAPP_KEYCODE_COMMA:
        pitch = 24;
        break;
    case SAPP_KEYCODE_L:
        pitch = 25;
        break;
    case SAPP_KEYCODE_PERIOD:
        pitch = 26;
        break;

        // upper keyboard
    case SAPP_KEYCODE_Q:
        pitch = 24;
        break;
    case SAPP_KEYCODE_2:
        pitch = 25;
        break;
    case SAPP_KEYCODE_W:
        pitch = 26;
        break;
    case SAPP_KEYCODE_3:
        pitch = 27;
        break;
    case SAPP_KEYCODE_E:
        pitch = 28;
        break;
    case SAPP_KEYCODE_RIGHT_SHIFT:
        pitch = 29;
        break;
    case SAPP_KEYCODE_5:
        pitch = 30;
        break;
    case SAPP_KEYCODE_T:
        pitch = 31;
        break;
    case SAPP_KEYCODE_6:
        pitch = 32;
        break;
    case SAPP_KEYCODE_Y:
        pitch = 33;
        break;
    case SAPP_KEYCODE_7:
        pitch = 34;
        break;
    case SAPP_KEYCODE_U:
        pitch = 35;
        break;
    case SAPP_KEYCODE_I:
        pitch = 36;
        break;
    case SAPP_KEYCODE_9:
        pitch = 37;
        break;
    case SAPP_KEYCODE_O:
        pitch = 38;
        break;
    case SAPP_KEYCODE_0:
        pitch = 39;
        break;
    case SAPP_KEYCODE_P:
        pitch = 40;
        break;
    }
    if (pitch > 0) {
        pitch += 2 * 12;
    }
    return pitch;
}

static void event(const sapp_event *ev)
{
    bool handled = simgui_handle_event(ev);
    if (!handled) {
        if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
            if (ev->key_code == SAPP_KEYCODE_ESCAPE) {
                sapp_quit();
            }
            else {
                int pitch = key_code_to_pitch(ev->key_code);
                printf("Note On  %d\n", pitch);
            }
        }
        else if (ev->type == SAPP_EVENTTYPE_KEY_UP) {
            int pitch = key_code_to_pitch(ev->key_code);
            printf("Note Off %d\n", pitch);
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
