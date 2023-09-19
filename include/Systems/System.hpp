#pragma once

#include "entt.hpp"

const std::string SCRIPT_PATH = "../Scripts/";

class System {
public:
    virtual bool update(entt::registry& reg, float delta_time) = 0;
};
