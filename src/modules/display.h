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
    int size;
    double *points;
} Point_set_t;

void line_chart(cairo_t *context, Line_Chart_t *chart, Point_set_t *point_set);

#endif
