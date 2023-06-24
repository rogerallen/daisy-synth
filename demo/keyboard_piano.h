#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"

typedef struct keyboard_piano_state {
    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float white_z;
    float black_z;
} keyboard_piano_state;

int key_code_to_pitch(sapp_keycode kc);
void draw_keyboard(sgl_pipeline pip_3d, int cur_pitch);