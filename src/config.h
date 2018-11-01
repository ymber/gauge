#ifndef CONFIG_H
#define CONFIG_H

#include <cairo.h>

typedef struct {
    int size_x;
    int size_y;
    double daemon_freq;
    double display_freq;
} Settings_t;

extern Settings_t settings;
extern void (*daemon_fncs[1])();
extern void (*display_fncs[4])(cairo_t*);

#endif
