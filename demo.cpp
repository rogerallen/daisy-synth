// using libdaisy requires C++
#include <daisysp.h>

#include "demo.h"

static daisysp::Oscillator osc;

static void cpp_daisy_init(int sample_rate)
{
    osc.Init((float)sample_rate);
    osc.SetFreq(440.0);
    osc.SetAmp(0.05);
    osc.SetWaveform(osc.WAVE_SIN);
}

static void cpp_stream_cb(float *buffer, int num_frames, int num_channels)
{
    assert(1 == num_channels);
    for (int i = 0; i < num_frames; i++)
    {
        buffer[i] = osc.Process();
    }
}

// bounce C-api calls into C++.  Probably a better way to do this...
extern "C"
{
    // the sample callback, running in audio thread
    void stream_cb(float *buffer, int num_frames, int num_channels)
    {
        cpp_stream_cb(buffer, num_frames, num_channels);
    }

    void daisy_init(int sample_rate)
    {
        cpp_daisy_init(sample_rate);
    }
}
