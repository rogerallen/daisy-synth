// Simplest Daisy Synth Demo
//
// GUI controls pitch, amplitude and waveform
// also provides audio callback.

#include "synth.h"

Synth::Synth(int sample_rate)
{
    gate_ = false;

    osc_pitch_ = -1;
    osc_wave_ = osc_.WAVE_SIN;
    osc_.Init((float)sample_rate);
    osc_.SetFreq(daisysp::mtof((float)osc_pitch_));
    osc_.SetAmp(1.0);
    osc_.SetWaveform(osc_wave_);

    env_attack_time_ = 0.01;
    env_decay_time_ = 0.01;
    env_sustain_level_ = 0.9;
    env_release_time_ = 0.01;
    env_.Init(sample_rate);
    env_.SetAttackTime(env_attack_time_);
    env_.SetDecayTime(env_decay_time_);
    env_.SetSustainLevel(env_sustain_level_);
    env_.SetReleaseTime(env_release_time_);

    flt_freq_ = 1800.0;
    flt_res_ = 0.9;
    flt_.Init((float)sample_rate);
    flt_.SetFreq(flt_freq_);
    flt_.SetRes(flt_res_);

    verb_feedback_ = 0.55;
    verb_lp_freq_ = 18000.0;
    verb_wet_factor_ = 0.5;
    verb_.Init((float)sample_rate);
    verb_.SetFeedback(verb_feedback_);
    verb_.SetLpFreq(verb_lp_freq_);

    comp_l_.Init((float)sample_rate);
    comp_r_.Init((float)sample_rate);
    // comp.SetAttack();
    // comp.SetMakeup();
    // comp.SetRatio();
    // comp.SetRelease();
    // comp.SetThreshold();

    pan_ = 0.0; // -1.0 - 1.0
}

// From Supercollider doc: Two channel equal power panner. Pan2 takes the square
// root of the linear scaling factor going from 1 (left or right) to 0.5.sqrt
// (~=0.707) in the center, which is about 3dB reduction.  Avoids problem
// inherent to linear panning is that the perceived volume of the signal drops
// in the middle.
// position ranges from [-1(left),1(right)]
// assumes v[0] is left and v[1] is right
inline void pan(float *v, float position)
{
    v[0] *= sqrtf((1.0f - position) / 2.0f);
    v[1] *= sqrtf((1.0f + position) / 2.0f);
}

// the sample callback, running in audio thread
void Synth::audio_cb(float *buffer, int num_frames, int num_channels)
{
    assert(2 == num_channels);
    for (int i = 0; i < num_frames; i++) {
        float dry = osc_.Process();

        // ADSR
        dry *= env_.Process(gate_);

        // rlpf
        dry = flt_.Process(dry);

        // reverb -> stereo output
        float wet[2];
        float mix[2];
        verb_.Process(dry, dry, &(wet[0]), &(wet[1]));
        mix[0] = (dry * (1.0 - verb_wet_factor_)) + (wet[0] * verb_wet_factor_);
        mix[1] = (dry * (1.0 - verb_wet_factor_)) + (wet[1] * verb_wet_factor_);

        // stereo compress
        mix[0] = comp_l_.Process(mix[0]);
        mix[1] = comp_r_.Process(mix[1]);

        // stereo pan
        pan(mix, pan_);

        buffer[2 * i + 0] = mix[0];
        buffer[2 * i + 1] = mix[1];
    }
}

void Synth::note_on(float pitch, float amplitude)
{
    osc_pitch_ = pitch;
    set_env_sustain_level(amplitude);
    osc_.SetFreq(daisysp::mtof(pitch));
    if (!gate_) {
        env_.Retrigger(false);
    }
    gate_ = true;
}

void Synth::note_off()
{
    gate_ = false;
    osc_pitch_ = -1;
}
