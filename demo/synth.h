// Simplest Daisy Synth Demo
#ifdef __cplusplus
extern "C" {
#endif
void synth_init(int sample_rate);
void audio_cb(float *buffer, int num_frames, int num_channels);
void note_on(int pitch, float amplitude);
void note_off(int pitch);
int get_pitch(void);
void set_wave(int wave);
#ifdef __cplusplus
}
#endif
