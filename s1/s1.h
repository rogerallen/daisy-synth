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
int get_pitch(void);
void set_wave(int wave);

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