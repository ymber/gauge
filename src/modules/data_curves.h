#ifndef DATA_CURVES_H
#define DATA_CURVES_H

#include <cairo.h>
#include "../overlay.h"

typedef struct {
    double r;
    double g;
    double b;
    double a;
} RGBA_t;

typedef struct {
    double (*data_fnc)();
    double max;
    RGBA_t bg_rgba;
    RGBA_t fg_rgba;
    int x;
    int y;
    int radius;
    int thickness;
    float start_angle;
    float end_angle;
} Data_Curve_t;

void draw_rings(cairo_t *context);

#endif
