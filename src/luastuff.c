#include <stdlib.h>
#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>

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

void luastuff(lua_State *L)
{
    call_lua(L, "mainfunc", "script.lua");
    lua_close(L);
}
