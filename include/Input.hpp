#pragma once

#include <unordered_map>
#include <string>

#include "SFML/Window.hpp"
#include "lua.hpp"

class Input {
private:
    inline static std::unordered_map<std::string, int> keys{
        { "NULL", -1 },
        { "A", sf::Keyboard::A },
        { "B", sf::Keyboard::B },
        { "C", sf::Keyboard::C },
        { "D", sf::Keyboard::D },
        { "E", sf::Keyboard::E },
        { "F", sf::Keyboard::F },
        { "G", sf::Keyboard::G },
        { "H", sf::Keyboard::H },
        { "I", sf::Keyboard::I },
        { "J", sf::Keyboard::J },
        { "K", sf::Keyboard::K },
        { "L", sf::Keyboard::L },
        { "M", sf::Keyboard::M },
        { "N", sf::Keyboard::N },
        { "O", sf::Keyboard::O },
        { "P", sf::Keyboard::P },
        { "R", sf::Keyboard::R },
        { "S", sf::Keyboard::S },
        { "T", sf::Keyboard::T },
        { "U", sf::Keyboard::U },
        { "V", sf::Keyboard::V },
        { "W", sf::Keyboard::W },
        { "X", sf::Keyboard::X },
        { "Y", sf::Keyboard::Y },
        { "Z", sf::Keyboard::Z },
        { "Num0", sf::Keyboard::Num0 },
        { "Num1", sf::Keyboard::Num1 },
        { "Num2", sf::Keyboard::Num2 },
        { "Num3", sf::Keyboard::Num3 },
        { "Num4", sf::Keyboard::Num4 },
        { "Num5", sf::Keyboard::Num5 },
        { "Num6", sf::Keyboard::Num6 },
        { "Num7", sf::Keyboard::Num7 },
        { "Num8", sf::Keyboard::Num8 },
        { "Num9", sf::Keyboard::Num9 },
        { "Escape", sf::Keyboard::Escape },
        { "LControl", sf::Keyboard::LControl },
        { "LShift", sf::Keyboard::LShift },
        { "LAlt", sf::Keyboard::LAlt },
        { "LSystem", sf::Keyboard::LSystem },
        { "RControl", sf::Keyboard::RControl },
        { "RShift", sf::Keyboard::RShift },
        { "RAlt", sf::Keyboard::RAlt },
        { "RSystem", sf::Keyboard::RSystem },
        { "Menu", sf::Keyboard::Menu },
        { "LBracket", sf::Keyboard::LBracket },
        { "RBracket", sf::Keyboard::RBracket },
        { "Semicolon", sf::Keyboard::Semicolon },
        { "Comma", sf::Keyboard::Comma },
        { "Period", sf::Keyboard::Period },
        { "Apostrophe", sf::Keyboard::Apostrophe },
        { "Slash", sf::Keyboard::Slash },
        { "Backslash", sf::Keyboard::Backslash },
        { "Grave", sf::Keyboard::Grave },
        { "Equal", sf::Keyboard::Equal },
        { "Hyphen", sf::Keyboard::Hyphen },
        { "Space", sf::Keyboard::Space },
        { "Enter", sf::Keyboard::Enter },
        { "Backspace", sf::Keyboard::Backspace },
    };

    inline static const std::unordered_map<std::string, int> mouse{
        { "Left", sf::Mouse::Left },
        { "Right", sf::Mouse::Right },
        { "Middle", sf::Mouse::Middle },
        { "XButton1", sf::Mouse::XButton1 },
        { "XButton2", sf::Mouse::XButton2 },
    };

    inline static bool m_key_pressed[59];

    // lua wrappers for input stuff
    static int lua_is_key_pressed(lua_State* L);

    static int lua_is_mouse_button_pressed(lua_State* L);
    static int lua_get_mouse_position(lua_State* L);

public:
    Input();
    virtual ~Input();

    static void lua_openinput(lua_State* L, Input* input);
    static void handle_event(sf::Event& event);
};
