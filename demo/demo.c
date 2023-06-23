//------------------------------------------------------------------------------
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "sokol_audio.h"
#include "sokol_gl.h"
#include "sokol_imgui.h"
#include <assert.h>
//------------------------------------------------------------------------------
#include "keyboard_piano.h"
#include "synth.h"

static struct {
    sg_pass_action pass_action;
    sgl_pipeline pip_3d;
    float cur_amplitude;
    int cur_wave;
    int next_wave;
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
        .stream_cb = audio_cb,
        .logger.func = slog_func,
    });
    synth_init(saudio_sample_rate());

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
    state.cur_amplitude = 0.05f;
    state.cur_wave = 0;
    state.next_wave = 0;
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
    igSetNextWindowSize((ImVec2){540, 125}, ImGuiCond_Once);
    igBegin("Controls", 0, ImGuiWindowFlags_None);
    igText("Press a key to make a Sound.");
    // igColorEdit3("Background",
    //              &state.pass_action.colors[0].clear_value.r,
    //              ImGuiColorEditFlags_None);
    igLabelText("Pitch", "%d", get_pitch());
    igSliderFloat("Amplitude", &state.cur_amplitude, 0.0, 1.0, "%.3f",
                  ImGuiSliderFlags_None);

    igRadioButton_IntPtr("SIN", &state.next_wave, 0);
    igSameLine(0.0f, -1.0f);
    igRadioButton_IntPtr("TRI", &state.next_wave, 1);
    igSameLine(0.0f, -1.0f);
    igRadioButton_IntPtr("SAW", &state.next_wave, 2);
    igSameLine(0.0f, -1.0f);
    igRadioButton_IntPtr("RAMP", &state.next_wave, 3);
    igSameLine(0.0f, -1.0f);
    igRadioButton_IntPtr("SQUARE", &state.next_wave, 4);
    igSameLine(0.0f, -1.0f);
    igRadioButton_IntPtr("PB_TRI", &state.next_wave, 5);
    igSameLine(0.0f, -1.0f);
    igRadioButton_IntPtr("PB_SAW", &state.next_wave, 6);
    igSameLine(0.0f, -1.0f);
    igRadioButton_IntPtr("PB_SQUARE", &state.next_wave, 7);

    igEnd();
    /*=== UI CODE ENDS HERE ===*/

    // sgl code start here
    draw_keyboard(state.pip_3d, get_pitch());

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

static void event(const sapp_event *ev)
{
    // handle wave radio box
    if (state.next_wave != state.cur_wave) {
        state.cur_wave = state.next_wave;
        set_wave(state.cur_wave);
    }
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
        .height = 400,
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}
