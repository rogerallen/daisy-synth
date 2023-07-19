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

    flt.Init((float)sample_rate);
    flt.SetFreq(1800);
    flt.SetRes(0.9);

    verb.Init((float)sample_rate);
    verb.SetFeedback(0.55);
    verb.SetLpFreq(18000.0);

    comp.Init((float)sample_rate);
    // comp.SetAttack();
    // comp.SetMakeup();
    // comp.SetRatio();
    // comp.SetRelease();
    // comp.SetThreshold();
}

// From Supercollider doc: Two channel equal power panner. Pan2 takes the square
// root of the linear scaling factor going from 1 (left or right) to 0.5.sqrt
// (~=0.707) in the center, which is about 3dB reduction.  Avoids problem
// inherent to linear panning is that the perceived volume of the signal drops
// in the middle.
// position ranges from [-1,1]
void pan(float *v, float position)
{
    v[0] *= sqrtf((1.0f - position) / 2.0f);
    v[1] *= sqrtf((1.0f + position) / 2.0f);
}

// the sample callback, running in audio thread
void Synth::audio_cb(float *buffer, int num_frames, int num_channels)
{
    assert(1 == num_channels); // FIXME
    for (int i = 0; i < num_frames; i++) {
        float dry = osc.Process();

        // filter it
        dry = flt.Process(dry);

        float wet_l, wet_r;
        verb.Process(dry, dry, &wet_l, &wet_r);
        float wet = (wet_l + wet_r) * 0.5f; // FIXME

        float reverb_factor = 0.5;
        float mix = (dry * (1.0 - reverb_factor)) + (wet * reverb_factor);

        mix = comp.Process(mix);

        buffer[i] = mix; // FIXME
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
    // printf("pitch=%f freq=%f\n", pitch, f);
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
