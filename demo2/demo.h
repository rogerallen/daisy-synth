#ifdef __cplusplus
extern "C" {
#endif

void demo_init(int sample_rate);
void demo_shutdown(void);

// synth
void audio_cb(float *buffer, int num_frames, int num_channels);
void note_on(int pitch, float amplitude);
void note_off(int pitch);
int get_pitch(void);
void set_wave(int wave);

// keyboard
int key_code_to_pitch(sapp_keycode kc);
void draw_keyboard(sgl_pipeline pip_3d, int cur_pitch);
int xy_to_pitch(float x, float y);

#ifdef __cplusplus
}
#endif
