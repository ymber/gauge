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

void line_chart(cairo_t *context, Line_Chart_t *chart, double *points);
void draw_chart(cairo_t *context);

#endif
