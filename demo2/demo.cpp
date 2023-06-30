//------------------------------------------------------------------------------
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"
//------------------------------------------------------------------------------
#include "demo.h"
#include "keyboard_piano.h"
#include "synth.h"

static Synth *synth;
static KeyboardPiano *keyboard_piano;

extern "C" {

void demo_init(int sample_rate)
{
    // initialize the synth
    synth = new Synth(sample_rate);
    keyboard_piano = new KeyboardPiano();
}

void demo_shutdown(void)
{
    delete synth;
    delete keyboard_piano;
}

// synth

void audio_cb(float *buffer, int num_frames, int num_channels)
{
    synth->audio_cb(buffer, num_frames, num_channels);
}

void note_on(int pitch, float amplitude) { synth->note_on(pitch, amplitude); }
void note_off(int pitch) { synth->note_off(pitch); }
int get_pitch(void) { return synth->get_pitch(); }
void set_wave(int wave) { synth->set_wave(wave); }

// keyboard

int xy_to_pitch(float x, float y) { return keyboard_piano->xy_to_pitch(x, y); }

int key_code_to_pitch(sapp_keycode kc)
{
    return keyboard_piano->key_code_to_pitch(kc);
}
void draw_keyboard(sgl_pipeline pip_3d, int cur_pitch)
{
    keyboard_piano->draw_keyboard(pip_3d, cur_pitch);
}
}
