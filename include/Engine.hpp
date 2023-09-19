#pragma once

#include <string>

#include "SFML/Graphics.hpp"
#include "lua.hpp"

#include "Scene.hpp"
#include "Input.hpp"

const float MAX_FPS = 120.0f;

class Engine {
private:
    sf::Vector2f m_dimensions;
    sf::RenderWindow m_window;
    std::string m_title;
    Scene m_scene;
    Input m_input;
    lua_State* L;
    sf::Clock m_clock;

    void lua_openmetatables(lua_State* L);
    void update(float delta_time);
    void render();

    float get_frame_time();

public:
    Engine(sf::Vector2u dimensions, std::string title);
    virtual ~Engine();

    void run();
};
