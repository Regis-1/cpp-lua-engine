#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"

#include "Resources.hpp"

struct C_Sprite {
    std::string texture_name;
    sf::IntRect texture_rect;
};

static C_Sprite lua_tosprite(lua_State* L, int index) {
    C_Sprite sprite_comp;

    if (!lua_istable(L, index)) {
        std::cout << "ERROR: Invalid sprite provided!" << std::endl;
        return sprite_comp;
    }

    lua_getfield(L, index, "name");
    sprite_comp.texture_name = lua_tostring(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "texture_rect");
    sprite_comp.texture_rect = lua_tointrect(L, -1);
    lua_pop(L, 1);

    return sprite_comp;
}

static void lua_pushsprite(lua_State* L, const C_Sprite& sprite_comp) {
    lua_newtable(L);

    lua_pushstring(L, sprite_comp.texture_name.c_str());
    lua_setfield(L, -2, "name");

    lua_pushintrect(L, sprite_comp.texture_rect);
    lua_setfield(L, -2, "texture_rect");
}
