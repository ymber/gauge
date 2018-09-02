#ifndef CONFIG_H
#define CONFIG_H

#include <cairo.h>

#include "overlay.h"
#include "modules/data_curves.h"

typedef struct {
    int size[2];
    int bg_rgba[4];
    int fg_rgba[4];
    int frequency;
} Settings_t;

Settings_t settings = {
    {1920, 1080},
    {0, 0, 1, 1},
    {1, 0, 1, 1},
    1
};

void (*call[1])(cairo_t*, Window*) = {
    draw_rings
};

#endif
