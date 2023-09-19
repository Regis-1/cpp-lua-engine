#include "Engine.hpp"

void Engine::lua_openmetatables(lua_State* L) {
    luaL_loadfile(L, "../scripts/vector.lua");
    lua_pcall(L, 0, 1, 0);
    lua_setglobal(L, "vector");
}

void Engine::update(float delta_time) {
    m_scene.update_systems(delta_time);
}

void Engine::render() {
    m_window.clear(sf::Color::Black);
    m_scene.render();
    m_window.display();
}

Engine::Engine(sf::Vector2u dimensions, std::string title)
    : m_dimensions(dimensions), m_title(title) {

        auto window_flags = sf::Style::Titlebar | sf::Style::Close;
        m_window.create(sf::VideoMode(dimensions), title, window_flags);

        m_scene.assign_window(&m_window);

        L = luaL_newstate();
        luaL_openlibs(L);
        this->lua_openmetatables(L);
        Scene::lua_openscene(L, &m_scene);
        Input::lua_openinput(L, &m_input);

        m_scene.create_system<S_Behaviour>(L);
        m_scene.create_system<S_Collision>(L);
        m_scene.set_scene(L, "scenes/main_scene.lua");
}

Engine::~Engine() {
    lua_close(L);
    m_window.close();
};

void Engine::run() {
    m_clock.restart();
    float delta_time = 0;

    while (m_window.isOpen() && !m_scene.should_quit()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            m_input.handle_event(event);
            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        if (m_clock.getElapsedTime().asSeconds() >= 1.0f / MAX_FPS) {
            delta_time = m_clock.restart().asSeconds();
            std::cout << delta_time << "s" << "  |  " << (int)(1/delta_time) <<
                "FPS" << std::endl;
            this->update(delta_time);
            this->render();
        }
    }
}
