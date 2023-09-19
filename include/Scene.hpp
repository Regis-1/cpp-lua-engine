#pragma once

#include <vector>
#include <string>

#include "entt.hpp"
#include "SFML/Graphics.hpp"

#include "Resources.hpp"
#include "Components/Components.hpp"
#include "Systems/Systems.hpp"

class Scene {
private:
    entt::registry m_reg;
    std::vector<System*> m_systems;
    Resources m_resources;
    bool m_should_quit;
    sf::RenderWindow* m_window{nullptr};

    inline static const std::vector<std::string> m_comp_types = {
        "Transform",
        "Sprite",
        "Behaviour"
    };

    // lua wrappers
    static int lua_set_scene(lua_State* L);
    static int lua_quit(lua_State* L);

    static int lua_get_entity_count(lua_State* L);
    static int lua_entity_valid(lua_State* L);
    static int lua_create_entity(lua_State* L);
    static int lua_remove_entity(lua_State* L);

    static int lua_has_component(lua_State* L);
    static int lua_get_component(lua_State* L);
    static int lua_set_component(lua_State* L);
    static int lua_remove_component(lua_State* L);

    static int lua_load_texture(lua_State* L);
    static int lua_unload_texture(lua_State* L);

public:
    Scene();
    virtual ~Scene();

    static void lua_openscene(lua_State* L, Scene* scene);
    void assign_window(sf::RenderWindow* window);

    void set_scene(lua_State* L, std::string path);
    void render();
    bool should_quit();

    template <typename T, typename ...Args>
    void create_system(Args... args);

    void update_systems(float delta_time);

    int get_entity_count() const;
    bool entity_valid(int entity) const;

    int create_entity();
    bool remove_entity(int entity);

    template <typename ...Args>
    bool has_components(int entity);

    template <typename T>
    T& get_component(int entity);

    template <typename T>
    void set_component(int entity, const T&);

    template <typename T, typename ...Args>
    void set_component(int entity, Args... args);

    template <typename T>
    void remove_component(int entity);

    bool load_texture(std::string name, std::string path,
            const sf::IntRect& int_rect = sf::IntRect());
    void unload_texture(std::string name);
    void add_sprite(int entity, std::string& name, sf::IntRect& int_rect);
};

template <typename T, typename ...Args>
void Scene::create_system(Args... args) {
    m_systems.emplace_back(new T(args...));
}

template <typename ...Args>
bool Scene::has_components(int entity) {
    return m_reg.all_of<Args...>((entt::entity)entity);
}

template <typename T>
T& Scene::get_component(int entity) {
    return m_reg.get<T>((entt::entity)entity);
}

template <typename T>
void Scene::set_component(int entity, const T& component) {
    auto test = m_reg.emplace_or_replace<T>((entt::entity)entity, component);
}

template <typename T, typename ...Args>
void Scene::set_component(int entity, Args... args) {
    m_reg.emplace_or_replace<T>((entt::entity)entity, args...);
}

template <typename T>
void Scene::remove_component(int entity) {
    m_reg.remove<T>((entt::entity)entity);
}
