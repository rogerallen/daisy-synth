// Simplest Daisy Synth Demo
//
// GUI controls pitch, amplitude and waveform
// also provides audio callback.

#include <daisysp.h>

#include "demo.h"

static daisysp::Oscillator osc;
static int cur_pitch = -1;

// using libdaisy requires C++, so this is in a C++ file
// To interface with C GUI code, wrap in extern "C"
extern "C" {

void synth_init(int sample_rate)
{
    osc.Init((float)sample_rate);
    osc.SetFreq(440.0);
    osc.SetAmp(0.0);
    osc.SetWaveform(osc.WAVE_SIN);
}

// the sample callback, running in audio thread
void audio_cb(float *buffer, int num_frames, int num_channels)
{
    assert(1 == num_channels);
    for (int i = 0; i < num_frames; i++) {
        buffer[i] = osc.Process();
    }
}

void note_on(int pitch, float amplitude)
{
    if (pitch != cur_pitch) {
        cur_pitch = pitch;
        osc.SetAmp(amplitude);
        osc.SetFreq(daisysp::mtof((float)pitch));
    }
}

void note_off(int pitch)
{
    if (pitch == cur_pitch) {
        osc.SetAmp(0.0);
        cur_pitch = -1;
    }
}

int get_pitch(void) { return cur_pitch; }

void set_wave(int wave) { osc.SetWaveform((uint8_t)wave); }
}
