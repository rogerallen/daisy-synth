//------------------------------------------------------------------------------
// Sokol C Application to C++ Launchpad.
//
// This file is for taking C API calls and converting them to C++ since
// the DaisySP is in C++ and related controls are also likely to be C++.
//
//------------------------------------------------------------------------------
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"
//------------------------------------------------------------------------------
#include "keyboard_piano.h"
#include "pitch_pressure_pad.h"
#include "s1.h"
#include "synth.h"

static Synth *synth;
static KeyboardPiano *keyboard_piano;
static PitchPressurePad *pitch_pressure_pad;

extern "C" {

void s1_init(int sample_rate)
{
    // initialize the synth
    synth = new Synth(sample_rate);
    keyboard_piano = new KeyboardPiano();
    pitch_pressure_pad = new PitchPressurePad();
}

void s1_shutdown(void)
{
    delete synth;
    delete keyboard_piano;
    delete pitch_pressure_pad;
}

// synth

void audio_cb(float *buffer, int num_frames, int num_channels)
{
    synth->audio_cb(buffer, num_frames, num_channels);
}

void note_on(float pitch, float amplitude) { synth->note_on(pitch, amplitude); }
void note_off() { synth->note_off(); }
float get_pitch() { return synth->get_pitch(); }
float get_frequency() { return synth->get_frequency(); }
int get_wave(void) { return synth->get_wave(); }
void set_wave(int wave) { synth->set_wave(wave); }
float get_env_attack_time() { return synth->get_env_attack_time(); }
void set_env_attack_time(float v) { synth->set_env_attack_time(v); }
float get_env_decay_time() { return synth->get_env_decay_time(); }
void set_env_decay_time(float v) { synth->set_env_decay_time(v); }
float get_env_sustain_level() { return synth->get_env_sustain_level(); }
void set_env_sustain_level(float v) { synth->set_env_sustain_level(v); }
float get_env_release_time() { return synth->get_env_release_time(); }
void set_env_release_time(float v) { synth->set_env_release_time(v); }
float get_flt_freq() { return synth->get_flt_freq(); }
void set_flt_freq(float v) { synth->set_flt_freq(v); }
float get_flt_res() { return synth->get_flt_res(); }
void set_flt_res(float v) { synth->set_flt_res(v); }
float get_verb_feedback() { return synth->get_verb_feedback(); }
void set_verb_feedback(float v) { synth->set_verb_feedback(v); }
float get_verb_lp_freq() { return synth->get_verb_lp_freq(); }
void set_verb_lp_freq(float v) { synth->set_verb_lp_freq(v); }
float get_verb_wet_factor() { return synth->get_verb_wet_factor(); }
void set_verb_wet_factor(float v) { synth->set_verb_wet_factor(v); }
float get_pan() { return synth->get_pan(); }
void set_pan(float v) { synth->set_pan(v); }

// keyboard

int xy_to_pitch(float x, float y) { return keyboard_piano->xy_to_pitch(x, y); }
int key_code_to_pitch(sapp_keycode kc)
{
    return keyboard_piano->key_code_to_pitch(kc);
}
void draw_keyboard(sgl_pipeline pip_3d, int cur_pitch)
{
    keyboard_piano->draw_keyboard(pip_3d, cur_pitch);
}

// pitch pressure pad

void draw_pad(sgl_pipeline pip_3d, pitch_pressure_t pp)
{
    pitch_pressure_pad->draw_pad(pip_3d, pp);
}
pitch_pressure_t pad_xy_to_pitch_pressure(float x, float y)
{
    return pitch_pressure_pad->pad_xy_to_pitch_pressure(x, y);
}
pitch_pressure_t pad_xy_to_pitch_bend_pressure(float factor, float start_x,
                                               float start_y, float x, float y)
{
    return pitch_pressure_pad->pad_xy_to_pitch_bend_pressure(factor, start_x,
                                                             start_y, x, y);
}

} // extern "C"
