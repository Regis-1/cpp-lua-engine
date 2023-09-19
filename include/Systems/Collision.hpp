#pragma once

#include "Dev/LuaUtils.hpp"
#include "Systems/System.hpp"

class S_Collision : public System {
private:
    lua_State* L;
public:
    S_Collision(lua_State* L) : L(L) {} 

    bool update(entt::registry& reg, float delta_time) final {
        return false;
    }
};
