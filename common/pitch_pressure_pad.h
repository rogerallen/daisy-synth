#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"

typedef struct pitch_pressure_t {
    float pitch;
    float pressure;
} pitch_pressure_t;

// pitch/pressure control
typedef struct pitch_pressure_state {
    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float margin_x;
    float margin_y;
    float base_z;
    float active_z;
    float min_pitch, max_pitch;
    float min_pressure, max_pressure;
} pitch_pressure_state;

void draw_pad(sgl_pipeline pip_3d, pitch_pressure_t pp);
pitch_pressure_t pad_xy_to_pitch_pressure(float x, float y);