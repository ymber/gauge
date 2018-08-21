#ifndef LUA_H
#define LUA_H

#include <lua.h>

typedef struct {
    int size[2];
    int bg_rgba[4];
    int fg_rgba[4];
    int frequency;
} Settings_t;

void luastuff();
void call_lua(lua_State*, const char*, const char*);
Settings_t load_settings(lua_State*, const char *);
void error(lua_State*, const char*, ...);

#endif