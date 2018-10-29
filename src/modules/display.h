#ifndef DISPLAY_H
#define DISPLAY_H

#include <cairo.h>

typedef struct {
    double max;
    int x;
    int y;
    int start_x;
    int start_y;
    int point_num;
} Line_Chart_t;

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

void line_chart(cairo_t *context, Line_Chart_t *chart, double *point_set);
void draw_ring(cairo_t *context, Data_Curve_t *arc, double coverage);

#endif
