#include "Input.hpp"
#include "Components/Components.hpp"

Input::Input() {}
Input::~Input() {}

void Input::lua_openinput(lua_State* L, Input* input) {
    lua_newtable(L);

    luaL_Reg methods[] = {
        { "IsKeyPressed", lua_is_key_pressed },
        { "IsMouseButtonPressed", lua_is_mouse_button_pressed },
        { "GetMousePosition", lua_get_mouse_position },
        { NULL, NULL }
    };

    lua_pushlightuserdata(L, input);
    luaL_setfuncs(L, methods, 1);
    lua_setglobal(L, "input");

    lua_newtable(L);
    for (auto& element : keys)
	{
		lua_pushnumber(L, element.second);
		lua_setfield(L, -2, element.first.c_str());
	}
	lua_setglobal(L, "Keys");

	lua_newtable(L);
	for (auto& element : mouse)
	{
		lua_pushnumber(L, element.second);
		lua_setfield(L, -2, element.first.c_str());
	}
	lua_setglobal(L, "Mouse");
}

void Input::handle_event(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed)
        m_key_pressed[(int)event.key.code] = true;
    else if (event.type == sf::Event::KeyReleased)
        m_key_pressed[(int)event.key.code] = false;
}

int Input::lua_is_key_pressed(lua_State* L) {
    int key = (int)lua_tointeger(L, 1);
    bool is_pressed = m_key_pressed[key];
    lua_pushboolean(L, is_pressed);
    return 1;
}

int Input::lua_is_mouse_button_pressed(lua_State* L) {
    int button = (int)lua_tointeger(L, 1);
    //should be Input::is_mouse_button_pressed()
    lua_pushboolean(L, sf::Mouse::isButtonPressed((sf::Mouse::Button)button));
    return 1;
}

int Input::lua_get_mouse_position(lua_State* L) {
    sf::Vector2i pos = sf::Mouse::getPosition();
    //should be Input::get_mouse_position()
    lua_pushvector(L, sf::Vector3f(pos.x, pos.y, 0.0f));
    return 1;
}
