// using libdaisy requires C++
#include <daisysp.h>

#include "demo.h"

static daisysp::Oscillator osc;

extern "C" {

void daisy_init(int sample_rate)
{
    osc.Init((float)sample_rate);
    osc.SetFreq(440.0);
    osc.SetAmp(0.05);
    osc.SetWaveform(osc.WAVE_SIN);
}

// the sample callback, running in audio thread
void stream_cb(float *buffer, int num_frames, int num_channels)
{
    assert(1 == num_channels);
    for (int i = 0; i < num_frames; i++) {
        buffer[i] = osc.Process();
    }
}
}
