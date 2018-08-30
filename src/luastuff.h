#ifndef LUA_H
#define LUA_H

#include <lua.h>

void luastuff(lua_State *L);
void call_lua(lua_State*, const char*, const char*);
void error(lua_State*, const char*, ...);

#endif
