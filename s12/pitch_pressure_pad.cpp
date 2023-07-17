#include "pitch_pressure_pad.h"

static float lerp(float x, float a, float b) { return a + x * (b - a); }

PitchPressurePad::PitchPressurePad()
{
    min_x = -1.0f;
    max_x = 1.0f;
    min_y = 0.45f;
    max_y = (6.0f / 8.0f);
    margin_x = 0.1f;
    margin_y = 0.1f; // top only
    base_z = -0.5f;
    active_z = -0.4f;
    min_pitch = 48.0; // (48/C3 to 76/E5)
    max_pitch = 77.0;
    min_pressure = 0.01;
    max_pressure = 0.9;
}

void PitchPressurePad::draw_pad(sgl_pipeline pip_3d, pitch_pressure_t pp)
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
    float x0 = min_x + margin_x;
    float x1 = max_x - margin_x;
    float y0 = min_y;
    float y1 = max_y - margin_y;
    int top_color = 150;
    int bot_color = 200;
    sgl_begin_quads();
    sgl_v3f_c3b(x0, y0, base_z, bot_color, bot_color, bot_color);
    sgl_v3f_c3b(x1, y0, base_z, bot_color, bot_color, bot_color);
    sgl_v3f_c3b(x1, y1, base_z, top_color, top_color, top_color);
    sgl_v3f_c3b(x0, y1, base_z, top_color, top_color, top_color);
    sgl_end();
    if (pp.pressure > 0.0f) {
        float ax = (pp.pitch - min_pitch) / (max_pitch - min_pitch);
        x1 = lerp(ax, x0, x1);
        float ay = (pp.pressure - min_pressure) / (max_pressure - min_pressure);
        y1 = lerp(ay, y0, y1);
        // printf("x1=%f, y1=%f\n", x1, y1);
        top_color = 200;
        bot_color = 255;
        sgl_begin_quads();
        sgl_v3f_c3b(x0, y0, active_z, bot_color, bot_color, bot_color);
        sgl_v3f_c3b(x1, y0, active_z, bot_color, bot_color, bot_color);
        sgl_v3f_c3b(x1, y1, active_z, top_color, top_color, top_color);
        sgl_v3f_c3b(x0, y1, active_z, top_color, top_color, top_color);
        sgl_end();
    }
}

pitch_pressure_t PitchPressurePad::pad_xy_to_pitch_pressure(float x, float y)
{
    // convert from 0-1 to sgl_ortho range
    float xx = x * 2 - 1;
    float yy = (1.0f - y) * (float)sapp_height() / sapp_width();
    // printf("%f, %f\n", xx, yy);

    pitch_pressure_t pp = pitch_pressure_t{}; //.pitch = 0.0f, .pressure = 0.0f};

    float start_x = min_x + margin_x;
    float end_x = max_x - margin_x;
    float start_y = min_y;
    float end_y = max_y - margin_y;
    if ((xx >= start_x) && (xx <= end_x) && (yy >= start_y) && (yy <= end_y)) {
        float ax = (xx - start_x) / (end_x - start_x);
        pp.pitch = lerp(ax, min_pitch, max_pitch);
        float ay = (yy - start_y) / (end_y - start_y);
        pp.pressure = lerp(ay, min_pressure, max_pressure);
        // printf("ax=%f ay=%f\n", ax, ay);
        // printf("pi=%f pr=%f\n", pp.pitch, pp.pressure);
    }
    return pp;
}