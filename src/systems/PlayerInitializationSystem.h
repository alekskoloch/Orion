#pragma once

#include <entt/entt.hpp>

class PlayerInitializationSystem
{
public:
    static void initializePlayer(entt::registry& registry);
};