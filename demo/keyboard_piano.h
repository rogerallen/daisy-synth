#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"

int key_code_to_pitch(sapp_keycode kc);
void draw_keyboard(sgl_pipeline pip_3d, int cur_pitch);