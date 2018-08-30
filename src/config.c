#include <lua.h>
#include <lauxlib.h>

#include "config.h"
#include "luastuff.h"

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
