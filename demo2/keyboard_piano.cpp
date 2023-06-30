#include "keyboard_piano.h"

#include <assert.h>
#include <stdio.h>

KeyboardPiano::KeyboardPiano()
{
    min_x = -1.0f;
    max_x = 1.0f;
    min_y = 0.0f;
    max_y = 0.5f;
    white_z = -0.5f;
    black_z = -0.4f;
    margin_x = 0.1f;
    margin_y = 0.1f;
};

// 3 octaves of indices for either black or white keys
// to convert pitch into an index into drawing either a white
// or black key.  -1 indicates "use the other array"
// positive value indicates "draw me"
static int white_key_idx_ary[] = {0,      -1, 1,      -1, 2,      3,      -1,
                                  4,      -1, 5,      -1, 6, //
                                  7 + 0,  -1, 7 + 1,  -1, 7 + 2,  7 + 3,  -1,
                                  7 + 4,  -1, 7 + 5,  -1, 7 + 6, //
                                  14 + 0, -1, 14 + 1, -1, 14 + 2, 14 + 3, -1,
                                  14 + 4, -1, 14 + 5, -1, 14 + 6};
static int black_key_idx_ary[] = {
    -1, 1,      -1, 3,      -1, -1, 6,      -1, 8,      -1, 10,      -1, //
    -1, 12 + 1, -1, 12 + 3, -1, -1, 12 + 6, -1, 12 + 8, -1, 12 + 10, -1, //
    -1, 24 + 1, -1, 24 + 3, -1, -1, 24 + 6, -1, 24 + 8, -1, 24 + 10, -1};
// go from x position to pitch
static int white_pitch_idx_ary[] = {
    0,      2,      4,      5,      7,      9,      11,      //
    12 + 0, 12 + 2, 12 + 4, 12 + 5, 12 + 7, 12 + 9, 12 + 11, //
    24 + 0, 24 + 2, 24 + 4, 24 + 5, 24 + 7, 24 + 9, 24 + 11,
};

// return midi pitch for keyboard events.  return -1 for no match
// range is 48/C3 to 76/E5
int KeyboardPiano::key_code_to_pitch(sapp_keycode kc)
{
    int pitch = -1;
    switch (kc) {
    case SAPP_KEYCODE_Z:
        pitch = 48; // C3
        break;
    case SAPP_KEYCODE_S:
        pitch = 49;
        break;
    case SAPP_KEYCODE_X:
        pitch = 50;
        break;
    case SAPP_KEYCODE_D:
        pitch = 51;
        break;
    case SAPP_KEYCODE_C:
        pitch = 52;
        break;
    case SAPP_KEYCODE_V:
        pitch = 53;
        break;
    case SAPP_KEYCODE_G:
        pitch = 54;
        break;
    case SAPP_KEYCODE_B:
        pitch = 55;
        break;
    case SAPP_KEYCODE_H:
        pitch = 56;
        break;
    case SAPP_KEYCODE_N:
        pitch = 57;
        break;
    case SAPP_KEYCODE_J:
        pitch = 58;
        break;
    case SAPP_KEYCODE_M:
        pitch = 59;
        break;
    case SAPP_KEYCODE_COMMA:
        pitch = 60; // C4 = Middle C
        break;
    case SAPP_KEYCODE_L:
        pitch = 61;
        break;
    case SAPP_KEYCODE_PERIOD:
        pitch = 62;
        break;

        // upper keyboard
    case SAPP_KEYCODE_Q:
        pitch = 60; // C4 = Middle C
        break;
    case SAPP_KEYCODE_2:
        pitch = 61;
        break;
    case SAPP_KEYCODE_W:
        pitch = 62;
        break;
    case SAPP_KEYCODE_3:
        pitch = 63;
        break;
    case SAPP_KEYCODE_E:
        pitch = 64;
        break;
    case SAPP_KEYCODE_R:
        pitch = 65;
        break;
    case SAPP_KEYCODE_5:
        pitch = 66;
        break;
    case SAPP_KEYCODE_T:
        pitch = 67;
        break;
    case SAPP_KEYCODE_6:
        pitch = 68;
        break;
    case SAPP_KEYCODE_Y:
        pitch = 69;
        break;
    case SAPP_KEYCODE_7:
        pitch = 70;
        break;
    case SAPP_KEYCODE_U:
        pitch = 71;
        break;
    case SAPP_KEYCODE_I:
        pitch = 72; // C5
        break;
    case SAPP_KEYCODE_9:
        pitch = 73;
        break;
    case SAPP_KEYCODE_O:
        pitch = 74;
        break;
    case SAPP_KEYCODE_0:
        pitch = 75;
        break;
    case SAPP_KEYCODE_P:
        pitch = 76; // E5
        break;
    }
    return pitch;
}

// draw a proper keyboard
// https://bootcamp.uxdesign.cc/drawing-a-flat-piano-keyboard-in-illustrator-de07c74a64c6?gi=866815891348
// a single octave is divided into seven diatonic steps and twelve chromatic
// steps. our range is 48/C3 to 76/E5.  But we will calculate things based on 3
// octaves [48,84) or 21 diatonic steps and 36 chromatic steps
void KeyboardPiano::draw_key(int pitch, bool active)
{
    // x ranges from -1 to 1
    float start_x = min_x + margin_x;
    float end_x = max_x - margin_x;
    float start_y = min_y + margin_y;
    float end_y = max_y - margin_y;
    float mid_y = (start_y + end_y) * 0.4f;
    // 7*2+3 white keys (48/C3 to 76/E5) fit between start_x & end_x
    float white_dx = (end_x - start_x) / (7 * 2 + 3);
    // 12 black keys fit within 7 white keys
    float black_dx = 7 * white_dx / 12;
    int p = pitch - 48;
    assert(sizeof(white_key_idx_ary) == sizeof(black_key_idx_ary));
    assert(p < sizeof(white_key_idx_ary));
    assert(p >= 0);
    int white_key_idx = white_key_idx_ary[p];
    int black_key_idx = black_key_idx_ary[p];
    // one *_key_idx should be -1, the other should be 0 or greater
    assert(((white_key_idx >= 0) && (black_key_idx < 0)) ||
           ((white_key_idx < 0) && (black_key_idx >= 0)));
    if (white_key_idx >= 0) {
        // draw a white key.  7 keys per octave
        float x0 = start_x + white_key_idx * white_dx;
        float x1 = x0 + white_dx * 0.95f; // leave a little in-between the keys
        float y0 = start_y;
        float y1 = end_y;
        int top_color = active ? 200 : 150;
        int bot_color = active ? 255 : 200;
        sgl_begin_quads();
        sgl_v3f_c3b(x0, y0, white_z, bot_color, bot_color, bot_color);
        sgl_v3f_c3b(x1, y0, white_z, bot_color, bot_color, bot_color);
        sgl_v3f_c3b(x1, y1, white_z, top_color, top_color, top_color);
        sgl_v3f_c3b(x0, y1, white_z, top_color, top_color, top_color);
        sgl_end();
    }
    else {
        // draw a black key.  12 keys per octave
        float x0 = start_x + black_key_idx * black_dx;
        float x1 = x0 + black_dx;
        float y0 = mid_y;
        float y1 = end_y;
        int top_color = active ? 40 : 120;
        int bot_color = active ? 50 : 150;
        sgl_begin_quads();
        sgl_v3f_c3b(x0, y0, black_z, bot_color, bot_color, bot_color);
        sgl_v3f_c3b(x1, y0, black_z, bot_color, bot_color, bot_color);
        sgl_v3f_c3b(x1, y1, black_z, top_color, top_color, top_color);
        sgl_v3f_c3b(x0, y1, black_z, top_color, top_color, top_color);
        sgl_end();
    }
}

void KeyboardPiano::draw_keyboard(sgl_pipeline pip_3d, int cur_pitch)
{
    sgl_defaults();
    sgl_viewport(0, 0, sapp_width(), sapp_height(), true);
    sgl_load_pipeline(pip_3d);
    sgl_matrix_mode_projection();
    sgl_ortho(
        -1.0, 1.0,                                // x ranges -1(left),1(right)
        0.0, (float)sapp_height() / sapp_width(), // y ranges 0(bottom),0.5(top)
        -1.0, 1.0);                               // z ranges -1(near), 1(far)
    // FIXME? On Windows z seems to range -1 (near) to 0 (far)
    sgl_matrix_mode_modelview();
    // draw keyboard
    for (int p = 3 * 12 + 12; p <= 3 * 12 + 40; p++) {
        draw_key(p, p == cur_pitch);
    }
    int err = sgl_error();
    if (err != SGL_NO_ERROR) {
        printf("SGL_ERROR = %d\n", err);
    }
}

int KeyboardPiano::xy_to_pitch(float x, float y)
{
    // convert from 0-1 to sgl_ortho range
    float xx = x * 2 - 1;
    float yy = (1.0f - y) * (float)sapp_height() / sapp_width();
    // printf("%f, %f\n", xx, yy);

    int pitch = -1;

    // x ranges from -1 to 1
    float start_x = min_x + margin_x;
    float end_x = max_x - margin_x;
    float start_y = min_y + margin_y;
    float end_y = max_y - margin_y;
    float mid_y = (start_y + end_y) * 0.4f;
    // 7*2+3 white keys (48/C3 to 76/E5) fit between start_x & end_x
    float white_dx = (end_x - start_x) / (7 * 2 + 3);
    // 12 black keys fit within 7 white keys
    float black_dx = 7 * white_dx / 12;
    if ((xx >= start_x) && (xx <= end_x) && (yy >= start_y) && (yy <= end_y)) {
        int white_pitch_idx = (int)((xx - start_x) / white_dx);
        int black_pitch_idx = (int)((xx - start_x) / black_dx);
        int white_pitch = white_pitch_idx_ary[white_pitch_idx];
        int black_pitch = black_key_idx_ary[black_pitch_idx];
        // printf("white_pitch=%d->%d black_pitch=%d->%d\n", white_pitch_idx,
        //        white_pitch, black_pitch_idx, black_pitch);
        if ((yy > mid_y) && black_pitch >= 0) {
            // printf("black pitch = %d\n", black_pitch);
            pitch = black_pitch + 48;
        }
        else if (white_pitch >= 0) {
            // printf("white pitch = %d\n", white_pitch);
            pitch = white_pitch + 48;
        }
    }
    return pitch;
}