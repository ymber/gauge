#ifndef DISPLAY_H
#define DISPLAY_H

#include <cairo.h>

#define M_PI 3.14159265358979323846

typedef struct {
    double r;
    double g;
    double b;
    double a;
} rgba_t;

typedef struct
{
    char *data;
    double max;
    rgba_t rgba;
    int thickness;
    int x;
    int y;
    int width;
    int height;
    int data_count;
} line_chart_t;

typedef struct
{
    char *data;
    double max;
    rgba_t bg_rgba;
    rgba_t fg_rgba;
    int thickness;
    int x;
    int y;
    int radius;
    double start_angle;
    double end_angle;
} data_arc_t;

void line_chart(cairo_t *context, line_chart_t *chart, double *point_set, int reverse);
void draw_ring(cairo_t *context, data_arc_t *arc, double coverage);

#endif
