#include <daisysp.h>

// Simplest Daisy Synth Demo
class Synth {
  public:
    Synth(int sample_rate);
    void audio_cb(float *buffer, int num_frames, int num_channels);
    void note_on(int pitch, float amplitude);
    void note_off(int pitch);
    int get_pitch(void);
    void set_wave(int wave);

  private:
    int cur_pitch;
    daisysp::Oscillator osc;
};