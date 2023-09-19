#pragma once

#include <iostream>
#include <unordered_map>
#include <utility>
#include "lua.hpp"
#include "SFML/Graphics.hpp"

const std::string RESOURCES_PATH = "../Resources/";

class Resources {
private:
    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<int, sf::Sprite> m_sprites;

public:
    Resources();
    ~Resources();

    bool load_texture(std::string name, std::string path,
            const sf::IntRect& int_rect);
    void unload_texture(std::string name);
    void create_sprite(int entity, std::string& name, sf::IntRect& int_rect);
    //void load_audio(std::string path); TODO...

    sf::Texture& get_texture(std::string name);
    sf::Sprite& get_sprite(int entity);
};

static sf::IntRect lua_tointrect(lua_State* L, int index) {
    if (!lua_istable(L, index))
        return sf::IntRect();

    int coords[4];
    for (int i = 0; i < 4; i++) {
        lua_pushinteger(L, i+1);
        lua_gettable(L, index-1);
        coords[i] = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
    }

    sf::Vector2i pos(coords[0], coords[1]);
    sf::Vector2i dims(coords[2], coords[3]);
    return sf::IntRect(pos, dims);
}

static void lua_pushintrect(lua_State* L, const sf::IntRect& int_rect) {
    lua_newtable(L);

    lua_pushinteger(L, 1); // index [1]
    lua_pushinteger(L, int_rect.left);
    lua_settable(L, -3);

    lua_pushinteger(L, 2); // index [2]
    lua_pushinteger(L, int_rect.top);
    lua_settable(L, -3);

    lua_pushinteger(L, 3); // index [3]
    lua_pushinteger(L, int_rect.width);
    lua_settable(L, -3);

    lua_pushinteger(L, 4); // index [4]
    lua_pushinteger(L, int_rect.height);
    lua_settable(L, -3);
}
