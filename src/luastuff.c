#include <stdlib.h>
#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "luastuff.h"

void error(lua_State *L, const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    exit(EXIT_FAILURE);
}

Settings_t load_settings(lua_State *L, const char *file)
{
    Settings_t settings;
    if(luaL_loadfile(L, file) || lua_pcall(L, 0, 0, 0)) {
        error(L, "%s\n", lua_tostring(L, -1));
    }
    lua_getglobal(L, "settings");
    lua_pushstring(L, "bg_rgba");
    lua_gettable(L, -2);
    for(int i = 1; i <= 4; ++i) {
        lua_pushinteger(L, i);
        lua_gettable(L, -2);
        printf("%f\n", lua_tonumber(L, -1));
        settings.bg_rgba[i - 1] = lua_tonumber(L, -1);
        lua_pop(L, 1);
    }
    return settings;
}

void call_lua(lua_State *L, const char *function, const char *file)
{
    printf("%s\n", function);
    if(luaL_loadfile(L, file) || lua_pcall(L, 0, 0, 0)) {
        error(L, "%s\n", lua_tostring(L, -1));
    }
    lua_getglobal(L, function);
    if(lua_pcall(L, 0, 0, 0) != LUA_OK) {
        error(L, "%s\n", lua_tostring(L, -1));
    }
}

void luastuff()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    load_settings(L, "config.lua");
    call_lua(L, "mainfunc", "script.lua");
    lua_close(L);
}
