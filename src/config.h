#ifndef CONFIG_H
#define CONFIG_H

#include <time.h>

#include <cairo.h>

typedef struct {
    int size_x;
    int size_y;
    struct timespec daemon_freq;
    struct timespec display_freq;
} Settings_t;

extern Settings_t settings;
extern void (*daemon_fncs[1])();
extern void (*display_fncs[1])(cairo_t*);

#endif
