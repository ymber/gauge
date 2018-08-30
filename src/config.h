#ifndef CONFIG_H
#define CONFIG_H

#include <lua.h>

typedef struct {
    int size[2];
    int bg_rgba[4];
    int fg_rgba[4];
    int frequency;
} Settings_t;

Settings_t load_settings(lua_State*, const char *);

#endif
