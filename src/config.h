#ifndef CONFIG_H
#define CONFIG_H

#include <time.h>

#include <cairo.h>
#include <cdynarr/cdynarr.h>

struct config
{
    int size_x;
    int size_y;
    struct timespec daemon_freq;
    struct timespec display_freq;
};

extern struct config settings;
extern vector_t *drawables;
extern void (*daemon_fncs[1])();
extern void (*display_fncs[1])(cairo_t *);

int read_config(char *config_file);
int read_drawables(char *config_file);
void clear_drawables();

#endif
