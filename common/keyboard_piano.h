#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"

class KeyboardPiano {
  public:
    KeyboardPiano();
    int key_code_to_pitch(sapp_keycode kc);
    void draw_keyboard(sgl_pipeline pip_3d, int cur_pitch);
    int xy_to_pitch(float x, float y);

  private:
    void draw_key(int pitch, bool active);
    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float white_z;
    float black_z;
    float margin_x;
    float margin_y;
};