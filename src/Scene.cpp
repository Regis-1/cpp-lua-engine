#include "Scene.hpp"

#include "Dev/LuaUtils.hpp"

Scene::Scene()
    : m_should_quit(false)
{
}

Scene::~Scene() {
}

void Scene::lua_openscene(lua_State* L, Scene* scene) {
    lua_newtable(L);

    luaL_Reg methods[] = {
        {"SetScene", lua_set_scene},
        {"Quit", lua_quit},
        {"GetEntityCount", lua_get_entity_count},
        {"EntityValid", lua_entity_valid},
        {"CreateEntity", lua_create_entity},
        {"RemoveEntity", lua_remove_entity},
        {"HasComponent", lua_has_component},
        {"GetComponent", lua_get_component},
        {"SetComponent", lua_set_component},
        {"RemoveComponent", lua_remove_component},
        {"LoadTexture", lua_load_texture},
        {"UnloadTexture", lua_unload_texture},
        {NULL, NULL}, // sentinel item
    };

    lua_pushlightuserdata(L, scene);
    luaL_setfuncs(L, methods, 1);
    lua_setglobal(L, "scene");

    lua_newtable(L);
    for (size_t i = 0; i < m_comp_types.size(); i++) {
        lua_pushnumber(L, (int)i);
        lua_setfield(L, -2, m_comp_types[i].c_str());
    }
    lua_setglobal(L, "ComponentType");
}

void Scene::assign_window(sf::RenderWindow* window) {
    m_window = window;
}

void Scene::set_scene(lua_State* L, std::string path) {
    m_reg.clear();
    if (luaL_dofile(L, (SCRIPT_PATH + path).c_str()) != LUA_OK)
        LUtils::dump_error(L);
}

void Scene::render() {
    auto view = m_reg.view<C_Transform, C_Sprite>();
    view.each([&](auto entity, auto& transform, auto& sprite){
        sf::Sprite& res_sprite = m_resources.get_sprite((int)entity);
        sf::Vector2f transform2d(transform.position.x, transform.position.y);
        res_sprite.setPosition(transform2d);
        res_sprite.setTextureRect(sprite.texture_rect);

        m_window->draw(res_sprite);
    });
}

bool Scene::should_quit() {
    return m_should_quit;
}

void Scene::update_systems(float delta_time) {
    for (auto it = m_systems.begin(); it != m_systems.end();)
        if ((*it)->update(m_reg, delta_time)) {
            delete (*it);
            it = m_systems.erase(it);
        }
        else {
            it++;
        }
}

int Scene::get_entity_count() const {
    return m_reg.alive();
}

bool Scene::entity_valid(int entity) const {
    return m_reg.valid((entt::entity)entity);
}

int Scene::create_entity() {
    int entity = (int)m_reg.create();
    this->set_component<C_Transform>(entity);
    return entity;
}

bool Scene::remove_entity(int entity) {
    bool valid = this->entity_valid(entity);
    if (valid)
        m_reg.destroy((entt::entity)entity);
    return valid;
}

bool Scene::load_texture(std::string name, std::string path,
        const sf::IntRect& int_rect) {
    return m_resources.load_texture(name, path, int_rect);
}

void Scene::unload_texture(std::string name) {
    m_resources.unload_texture(name);
}

void Scene::add_sprite(int entity, std::string& name, sf::IntRect& int_rect) {
    m_resources.create_sprite(entity, name, int_rect);
}

int Scene::lua_set_scene(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    std::string path = lua_tostring(L, 1);
    scene->set_scene(L, path);

    return 0;
}

int Scene::lua_quit(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    scene->m_should_quit = true;
    return 0;
}

int Scene::lua_get_entity_count(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    lua_pushnumber(L, scene->get_entity_count());
    return 1;
}

int Scene::lua_entity_valid(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    lua_pushboolean(L, scene->entity_valid((int)lua_tointeger(L, 1)));
    return 1;
}

int Scene::lua_create_entity(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    lua_pushnumber(L, scene->create_entity());
    return 1;
}

int Scene::lua_remove_entity(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    lua_pushboolean(L, scene->remove_entity((int)lua_tointeger(L, 1)));
    return 1;
}

int Scene::lua_has_component(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    int entity = (int)lua_tointeger(L, 1);
    int type = (int)lua_tointeger(L, 2);
    bool has_type = false;

    if (m_comp_types.at(type) == "Transform")
        has_type = scene->has_components<C_Transform>(entity);
    else if (m_comp_types.at(type) == "Behaviour")
        has_type = scene->has_components<C_Behaviour>(entity);
    else if (m_comp_types.at(type) == "Sprite")
        has_type = scene->has_components<C_Sprite>(entity);

    lua_pushboolean(L, has_type);
    return 1;
}

int Scene::lua_get_component(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    int entity = (int)lua_tointeger(L, 1);
    int type = (int)lua_tointeger(L, 2);

    if (m_comp_types.at(type) == "Transform" && scene->has_components<C_Transform>(entity))
        lua_pushtransform(L, scene->get_component<C_Transform>(entity));
    else if (m_comp_types.at(type) == "Behaviour" && scene->has_components<C_Behaviour>(entity))
        lua_rawgeti(L, LUA_REGISTRYINDEX, scene->get_component<C_Behaviour>(entity).lua_ref);
    else if (m_comp_types.at(type) == "Sprite" && scene->has_components<C_Sprite>(entity))
        lua_pushsprite(L, scene->get_component<C_Sprite>(entity));
    else
        lua_pushnil(L);

    return 1;
}

int Scene::lua_set_component(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    int entity = (int)lua_tointeger(L, 1);
    int type = (int)lua_tointeger(L, 2);

    if (m_comp_types.at(type) == "Transform")
        scene->set_component<C_Transform>(entity, lua_totransform(L, 3));
    else if (m_comp_types.at(type) == "Sprite") {
        scene->set_component<C_Sprite>(entity, lua_tosprite(L, 3));
        C_Sprite& sprite_comp = scene->get_component<C_Sprite>(entity);
        scene->add_sprite(entity, sprite_comp.texture_name, sprite_comp.texture_rect);
    }
    else if (m_comp_types.at(type) == "Behaviour") {
        if (scene->has_components<C_Behaviour>(entity))
            scene->remove_component<C_Behaviour>(entity);

        std::string path = lua_tostring(L, 3);
        if (luaL_dofile(L, (SCRIPT_PATH + path).c_str()) != LUA_OK)
            LUtils::dump_error(L);
        else {
            lua_pushvalue(L, -1);
            int lua_ref = luaL_ref(L, LUA_REGISTRYINDEX);

            lua_pushinteger(L, entity);
            lua_setfield(L, -2, "ID");

            lua_pushstring(L, path.c_str());
            lua_setfield(L, -2, "path");

            lua_getfield(L, -1, "init");
            if (lua_type(L, -1) == LUA_TNIL)
            {
                lua_pop(L, 1);
                return 0;
            }

            lua_pushvalue(L, -2);
            if (lua_pcall(L, 1, 0, 0) != LUA_OK)
                LUtils::dump_error(L);
            else {
                scene->set_component<C_Behaviour>(entity, path.c_str(), lua_ref);
                return 1;
            }
        }
    }

    return 0;
}

int Scene::lua_remove_component(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    int entity = (int)lua_tointeger(L, 1);
    int type = (int)lua_tointeger(L, 2);

    if (m_comp_types.at(type) == "Behaviour" && scene->has_components<C_Behaviour>(entity))
        scene->remove_component<C_Behaviour>(entity);
    else if (m_comp_types.at(type) == "Sprite" && scene->has_components<C_Sprite>(entity))
        scene->remove_component<C_Sprite>(entity);


    return 0;
}

int Scene::lua_load_texture(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    std::string name = lua_tostring(L, 1);
    std::string path = lua_tostring(L, 2);

    bool result;
    result = scene->load_texture(name, path, lua_tointrect(L, 3));

    lua_pushboolean(L, result);
    return 1;
}

int Scene::lua_unload_texture(lua_State* L) {
    Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
    std::string name = lua_tostring(L, 1);

    scene->unload_texture(name);

    return 0;
}
