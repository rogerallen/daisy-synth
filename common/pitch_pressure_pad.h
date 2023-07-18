#pragma once

#include "pitch_pressure_t.h"
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"

class PitchPressurePad {
  public:
    PitchPressurePad();
    void draw_pad(sgl_pipeline pip_3d, pitch_pressure_t pp);
    pitch_pressure_t pad_xy_to_pitch_pressure(float x, float y);
    pitch_pressure_t pad_xy_to_pitch_bend_pressure(float factor, float start_x,
                                                   float start_y, float x,
                                                   float y);

  private:
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

    float start_pitch;
};
