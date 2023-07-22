#pragma once

#include "pitch_pressure_t.h"

#ifdef __cplusplus
extern "C" {
#endif

void s1_init(int sample_rate);
void s1_shutdown(void);

// synth
void audio_cb(float *buffer, int num_frames, int num_channels);
void note_on(float pitch, float amplitude);
void note_off(void);
float get_pitch(void);
float get_frequency(void);
int get_wave(void);
void set_wave(int wave);
float get_env_attack_time();
void set_env_attack_time(float v);
float get_env_decay_time();
void set_env_decay_time(float v);
float get_env_sustain_level();
void set_env_sustain_level(float v);
float get_env_release_time();
void set_env_release_time(float v);
float get_flt_freq();
void set_flt_freq(float v);
float get_flt_res();
void set_flt_res(float v);
float get_verb_feedback();
void set_verb_feedback(float v);
float get_verb_lp_freq();
void set_verb_lp_freq(float v);
float get_verb_wet_factor();
void set_verb_wet_factor(float v);
float get_pan();
void set_pan(float v);

// keyboard
int key_code_to_pitch(sapp_keycode kc);
void draw_keyboard(sgl_pipeline pip_3d, int cur_pitch);
int xy_to_pitch(float x, float y);

// pitch_pressure pad
void draw_pad(sgl_pipeline pip_3d, pitch_pressure_t pp);
pitch_pressure_t pad_xy_to_pitch_pressure(float x, float y);
pitch_pressure_t pad_xy_to_pitch_bend_pressure(float factor, float start_x,
                                               float start_y, float x, float y);
#ifdef __cplusplus
}
#endif