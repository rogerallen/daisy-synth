#pragma once

#include <daisysp.h>

// Simplest Daisy Synth Demo
class Synth {
  public:
    Synth(int sample_rate);
    void audio_cb(float *buffer, int num_frames, int num_channels);

    // set freq/amp params
    void note_on(float pitch, float amplitude);
    void note_off(void);
    // getter setters
    int get_pitch(void) { return osc_pitch_; }
    int get_wave() { return osc_wave_; }
    void set_wave(int wave)
    {
        osc_wave_ = wave;
        osc_.SetWaveform((uint8_t)wave);
    }
    float get_env_attack_time() { return env_attack_time_; }
    void set_env_attack_time(float v)
    {
        env_attack_time_ = v;
        env_.SetAttackTime(env_attack_time_);
    }
    float get_env_decay_time() { return env_decay_time_; }
    void set_env_decay_time(float v)
    {
        env_decay_time_ = v;
        env_.SetDecayTime(env_decay_time_);
    }
    float get_env_sustain_level() { return env_sustain_level_; }
    void set_env_sustain_level(float v)
    {
        env_sustain_level_ = v;
        env_.SetSustainLevel(env_sustain_level_);
    }
    float get_env_release_time() { return env_release_time_; }
    void set_env_release_time(float v)
    {
        env_release_time_ = v;
        env_.SetReleaseTime(env_release_time_);
    }

    float get_flt_freq() { return flt_freq_; }
    void set_flt_freq(float v)
    {
        flt_freq_ = v;
        flt_.SetFreq(flt_freq_);
    }
    float get_flt_res() { return flt_res_; }
    void set_flt_res(float v)
    {
        flt_res_ = v;
        flt_.SetRes(flt_res_);
    }

    float get_verb_feedback() { return verb_feedback_; }
    void set_verb_feedback(float v)
    {
        verb_feedback_ = v;
        verb_.SetFeedback(verb_feedback_);
    }
    float get_verb_lp_freq() { return verb_lp_freq_; }
    void set_verb_lp_freq(float v)
    {
        verb_lp_freq_ = v;
        verb_.SetFeedback(verb_lp_freq_);
    }
    float get_verb_wet_factor() { return verb_wet_factor_; }
    void set_verb_wet_factor(float v)
    {
        verb_wet_factor_ = v;
        verb_.SetFeedback(verb_wet_factor_);
    }

    float get_pan() { return pan_; }
    void set_pan(float v) { pan_ = v; }

  private:
    bool gate_;
    daisysp::Oscillator osc_;
    int osc_pitch_; // FIXME float pitch
    int osc_wave_;
    daisysp::Adsr env_;
    float env_attack_time_;
    float env_decay_time_;
    float env_sustain_level_;
    float env_release_time_;
    daisysp::MoogLadder flt_;
    float flt_freq_;
    float flt_res_;
    daisysp::ReverbSc verb_;
    float verb_feedback_;
    float verb_lp_freq_;
    float verb_wet_factor_;
    daisysp::Compressor comp_l_, comp_r_;
    float pan_;
};