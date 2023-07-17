// Simplest Daisy Synth Demo
//
// GUI controls pitch, amplitude and waveform
// also provides audio callback.

#include "synth.h"

Synth::Synth(int sample_rate)
{
    cur_pitch = -1;
    osc.Init((float)sample_rate);
    osc.SetFreq(440.0);
    osc.SetAmp(0.0);
    osc.SetWaveform(osc.WAVE_SIN);
}

// the sample callback, running in audio thread
void Synth::audio_cb(float *buffer, int num_frames, int num_channels)
{
    assert(1 == num_channels);
    for (int i = 0; i < num_frames; i++) {
        buffer[i] = osc.Process();
    }
}

void Synth::note_on(int pitch, float amplitude)
{
    if (pitch != cur_pitch) {
        cur_pitch = pitch;
        osc.SetAmp(amplitude);
        osc.SetFreq(daisysp::mtof((float)pitch));
    }
}

void Synth::note_on_f(float pitch, float amplitude)
{
    cur_pitch = pitch;
    osc.SetAmp(amplitude);
    float f = daisysp::mtof(pitch);
    osc.SetFreq(f);
    printf("freq=%f\n", f);
}

void Synth::note_off(int pitch)
{
    if (pitch == cur_pitch) {
        osc.SetAmp(0.0);
        cur_pitch = -1;
    }
}

void Synth::note_off_f(void)
{
    osc.SetAmp(0.0);
    cur_pitch = -1;
}

int Synth::get_pitch(void) { return cur_pitch; }

void Synth::set_wave(int wave) { osc.SetWaveform((uint8_t)wave); }
