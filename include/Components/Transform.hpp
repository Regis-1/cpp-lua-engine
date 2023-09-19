#pragma once

#include <iostream>

#include "SFML/System/Vector3.hpp"
#include "lua.hpp"

struct C_Transform {
    sf::Vector3f position;
    sf::Vector3f rotation;
    sf::Vector3f scale;

    C_Transform()
        : position(sf::Vector3f()), rotation(sf::Vector3f()), scale(sf::Vector3f())
    {}
};

static sf::Vector3f lua_tovector(lua_State* L, int index) {
    sf::Vector3f vec;

    if (!lua_istable(L, index)) {
        std::cout << "Error: not table" << std::endl;
        return vec;
    }

    lua_getfield(L, index, "x");
    vec.x = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    vec.y = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    lua_getfield(L, index, "z");
    vec.z = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    return vec;
}

static void lua_pushvector(lua_State* L, const sf::Vector3f& vec) {
    lua_newtable(L);

    lua_pushnumber(L, vec.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, vec.y);
    lua_setfield(L, -2, "y");

    lua_pushnumber(L, vec.z);
    lua_setfield(L, -2, "z");

    lua_getglobal(L, "vector");
    lua_setmetatable(L, -2);
}

static C_Transform lua_totransform(lua_State* L, int index) {
    C_Transform transform;

    if(!lua_istable(L, index)) {
        std::cout << "Error: not table" << std::endl;
        return transform;
    }

    lua_getfield(L, index, "position");
    transform.position = lua_tovector(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "rotation");
    transform.rotation = lua_tovector(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "scale");
    transform.scale = lua_tovector(L, -1);
    lua_pop(L, 1);

    return transform;
}

static void lua_pushtransform(lua_State* L, const C_Transform& transform) {
    lua_newtable(L);

    lua_pushvector(L, transform.position);
    lua_setfield(L, -2, "position");

    lua_pushvector(L, transform.rotation);
    lua_setfield(L, -2, "rotation");

    lua_pushvector(L, transform.scale);
    lua_setfield(L, -2, "scale");
}
