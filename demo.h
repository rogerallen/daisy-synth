#ifdef __cplusplus
extern "C" {
#endif
void stream_cb(float *buffer, int num_frames, int num_channels);
void daisy_init(int sample_rate);
void note_on(int pitch, float amplitude);
void note_off(int pitch);
int get_pitch(void);
#ifdef __cplusplus
}
#endif
