#ifndef CONFIG_H
#define CONFIG_H

#include <cairo.h>

#include "overlay.h"
#include "modules/data_curves.h"

typedef struct {
    int size_x;
    int size_y;
    int frequency;
} Settings_t;

Settings_t settings = {
    1024,
    768,
    1
};

void (*call[1])(cairo_t*, Window*) = {
    draw_rings
};

#endif
