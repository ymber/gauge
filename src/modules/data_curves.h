#ifndef DATA_CURVES_H
#define DATA_CURVES_H

#include <cairo.h>

void seconds_ring(cairo_t *context);
void memory_ring(cairo_t *context);
void cpu_rings(cairo_t *context);
void disk_usage_rings(cairo_t *context);

void draw_rings(cairo_t *context);

#endif
