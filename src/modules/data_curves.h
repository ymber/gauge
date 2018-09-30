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
    double max;
    RGBA_t bg_rgba;
    RGBA_t fg_rgba;
    int x;
    int y;
    int radius;
    int thickness;
    double start_angle;
    double end_angle;
} Data_Curve_t;

void seconds_ring(cairo_t *context);
void memory_ring(cairo_t *context);
void cpu_rings(cairo_t *context);

void draw_ring(cairo_t *context, Data_Curve_t *arc, double coverage);
void draw_rings(cairo_t *context);

#endif
