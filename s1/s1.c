//------------------------------------------------------------------------------
// Sokol C Application
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
#include "s1.h"

static struct {
    sg_pass_action pass_action;
    sgl_pipeline pip_3d;
    float cur_amplitude;
    int cur_wave;
    int next_wave;
    sapp_event_type cur_event_type;
    bool mouse_down;
    int start_mouse_x, start_mouse_y;
    int mouse_x, mouse_y;
    int mouse_pitch;
    pitch_pressure_t pad;
    float pitch_bend_factor;
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
        .num_channels = 2, // stereo
        .stream_cb = audio_cb,
        .logger.func = slog_func,
    });
    s1_init(saudio_sample_rate());

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
    state.cur_amplitude = 0.5f;
    state.cur_wave = 0;
    state.next_wave = 0;
    state.cur_event_type = 0;
    state.mouse_down = false;
    state.start_mouse_x = -1;
    state.start_mouse_y = -1;
    state.mouse_x = -1;
    state.mouse_y = -1;
    state.mouse_pitch = -1;
    state.pad = (pitch_pressure_t){.pitch = 0.0f, .pressure = 0.0f};
    state.pitch_bend_factor = 3.0f;
}

static const char *eventtype_to_str(sapp_event_type t)
{
    switch (t) {
    case SAPP_EVENTTYPE_INVALID:
        return "INVALID";
    case SAPP_EVENTTYPE_KEY_DOWN:
        return "KEY_DOWN";
    case SAPP_EVENTTYPE_KEY_UP:
        return "KEY_UP";
    case SAPP_EVENTTYPE_CHAR:
        return "CHAR";
    case SAPP_EVENTTYPE_MOUSE_DOWN:
        return "MOUSE_DOWN";
    case SAPP_EVENTTYPE_MOUSE_UP:
        return "MOUSE_UP";
    case SAPP_EVENTTYPE_MOUSE_SCROLL:
        return "MOUSE_SCROLL";
    case SAPP_EVENTTYPE_MOUSE_MOVE:
        return "MOUSE_MOVE";
    case SAPP_EVENTTYPE_MOUSE_ENTER:
        return "MOUSE_ENTER";
    case SAPP_EVENTTYPE_MOUSE_LEAVE:
        return "MOUSE_LEAVE";
    case SAPP_EVENTTYPE_TOUCHES_BEGAN:
        return "TOUCHES_BEGAN";
    case SAPP_EVENTTYPE_TOUCHES_MOVED:
        return "TOUCHES_MOVED";
    case SAPP_EVENTTYPE_TOUCHES_ENDED:
        return "TOUCHES_ENDED";
    case SAPP_EVENTTYPE_TOUCHES_CANCELLED:
        return "TOUCHES_CANCELLED";
    case SAPP_EVENTTYPE_RESIZED:
        return "RESIZED";
    case SAPP_EVENTTYPE_ICONIFIED:
        return "ICONIFIED";
    case SAPP_EVENTTYPE_RESTORED:
        return "RESTORED";
    case SAPP_EVENTTYPE_FOCUSED:
        return "FOCUSED";
    case SAPP_EVENTTYPE_UNFOCUSED:
        return "UNFOCUSED";
    case SAPP_EVENTTYPE_SUSPENDED:
        return "SUSPENDED";
    case SAPP_EVENTTYPE_RESUMED:
        return "RESUMED";
    case SAPP_EVENTTYPE_QUIT_REQUESTED:
        return "QUIT_REQUESTED";
    case SAPP_EVENTTYPE_CLIPBOARD_PASTED:
        return "CLIPBOARD_PASTED";
    case SAPP_EVENTTYPE_FILES_DROPPED:
        return "FILES_DROPPED";
    default:
        return "???";
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
    igSetNextWindowSize((ImVec2){540, 200}, ImGuiCond_Once);
    igBegin("Controls", 0, ImGuiWindowFlags_None);
    igText("Press a key to make a Sound.");
    // igColorEdit3("Background",
    //              &state.pass_action.colors[0].clear_value.r,
    //              ImGuiColorEditFlags_None);
    // igLabelText("EventType", "%s", eventtype_to_str(state.cur_event_type));
    igLabelText("Pitch", "%d", get_pitch());
    igLabelText("PadPitch", "%f", state.pad.pitch);
    igLabelText("PadPressure", "%f", state.pad.pressure);
    igSliderFloat("Amplitude", &state.cur_amplitude, 0.0, 1.0, "%.3f",
                  ImGuiSliderFlags_None);
    igSliderFloat("PitchBendFactor", &state.pitch_bend_factor, 0.0, 12.0,
                  "%.3f", ImGuiSliderFlags_None);
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

    draw_pad(state.pip_3d, state.pad);

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
    state.cur_event_type = ev->type;

    // handle wave radio box
    if (state.next_wave != state.cur_wave) {
        state.cur_wave = state.next_wave;
        set_wave(state.cur_wave);
    }
    bool handled = simgui_handle_event(ev);
    // always handle key events
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
        float pitch = key_code_to_pitch(ev->key_code);
        if (pitch > 0) {
            // printf("Note Off %d\n", pitch);
            note_off();
        }
    }
    // allow mouse events to be blocked by gui
    if (!handled) {
        if ((ev->type == SAPP_EVENTTYPE_MOUSE_DOWN) &&
            (ev->mouse_button == SAPP_MOUSEBUTTON_LEFT)) {
            state.mouse_down = true;
            state.start_mouse_x = state.mouse_x = ev->mouse_x;
            state.start_mouse_y = state.mouse_y = ev->mouse_y;
            state.pad =
                pad_xy_to_pitch_pressure((float)state.mouse_x / sapp_width(),
                                         (float)state.mouse_y / sapp_height());
            if (state.pad.pressure > 0.0) {
                note_on(state.pad.pitch, state.pad.pressure);
            }
            else {
                state.mouse_pitch =
                    xy_to_pitch((float)state.mouse_x / sapp_width(),
                                (float)state.mouse_y / sapp_height());
                if (state.mouse_pitch > 0) {
                    note_on(state.mouse_pitch, state.cur_amplitude);
                }
            }
        }
        else if ((ev->type == SAPP_EVENTTYPE_MOUSE_UP) &&
                 (ev->mouse_button == SAPP_MOUSEBUTTON_LEFT)) {
            state.mouse_down = false;
            state.mouse_x = ev->mouse_x;
            state.mouse_y = ev->mouse_y;
            if (state.mouse_pitch > 0) {
                note_off();
                state.mouse_pitch = -1;
            }
            if (state.pad.pressure > 0.0) {
                state.pad.pitch = 0.0f;
                state.pad.pressure = 0.0f;
                note_off();
            }
        }
        else if ((ev->type == SAPP_EVENTTYPE_MOUSE_MOVE) && state.mouse_down) {
            state.mouse_x = ev->mouse_x;
            state.mouse_y = ev->mouse_y;
            // printf("mouse x=%d y=%d ", state.mouse_x, state.mouse_y);
            state.pad = pad_xy_to_pitch_bend_pressure(
                state.pitch_bend_factor,
                (float)state.start_mouse_x / sapp_width(),
                (float)state.start_mouse_y / sapp_height(),
                (float)state.mouse_x / sapp_width(),
                (float)state.mouse_y / sapp_height());
            if (state.pad.pressure > 0.0) {
                note_on(state.pad.pitch, state.pad.pressure);
            }
            else {
                float pitch =
                    (float)xy_to_pitch((float)state.mouse_x / sapp_width(),
                                       (float)state.mouse_y / sapp_height());
                if ((state.mouse_pitch > 0) && (pitch > 0) &&
                    (pitch != state.mouse_pitch)) {
                    state.mouse_pitch = pitch;
                    note_on(state.mouse_pitch, state.cur_amplitude);
                }
            }
        }
    }
    // MOUSE_LEAVE/ENTER?
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
        .window_title = "daisy-synth:demo",
        .width = 800,
        .height = 900,
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}
