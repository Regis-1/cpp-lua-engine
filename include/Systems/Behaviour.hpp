#pragma once

#include "Dev/LuaUtils.hpp"
#include "Systems/System.hpp"
#include "Components/Behaviour.hpp"

class S_Behaviour : public System {
private:
    lua_State* L;
public:
    S_Behaviour(lua_State* L) : L(L) {}

    bool update(entt::registry& reg, float delta_time) final {
        auto view = reg.view<C_Behaviour>();

        view.each([&](const C_Behaviour& script){
            lua_rawgeti(L, LUA_REGISTRYINDEX, script.lua_ref);
            if (luaL_dofile(L, (SCRIPT_PATH + std::string(script.path)).c_str()) != LUA_OK)
                LUtils::dump_error(L);
            else {
                lua_getfield(L, -1, "update");
                lua_setfield(L, -3, "update");
                lua_pop(L, 1);
            }

            lua_getfield(L, -1, "update");
            if (lua_type(L, -1) == LUA_TNIL) {
                lua_pop(L, -1);
                return;
            }

            lua_pushvalue(L, -2);
            lua_pushnumber(L, delta_time);

            if (lua_pcall(L, 2, 0, 0) != LUA_OK)
                LUtils::dump_error(L);
            else
                lua_pop(L, 1);
        });

        return false;
    }
};
