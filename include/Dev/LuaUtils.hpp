#pragma once

#include <string>
#include "lua.hpp"

namespace LUtils {
    static void dump_error(lua_State* L) {
        if (lua_gettop(L) && lua_isstring(L, -1)) {
            std::cout << "LUA_ERROR: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }
    }
}
