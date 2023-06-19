#ifdef __cplusplus
extern "C"
{
#endif
    void stream_cb(float *buffer, int num_frames, int num_channels);
    void daisy_init(int sample_rate);
#ifdef __cplusplus
}
#endif
