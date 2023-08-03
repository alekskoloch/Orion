#pragma once

#include <entt/entt.hpp>

#include "../schema/WeaponsSchema.h"

class PlayerInitializationSystem
{
public:
    static void initializePlayer(entt::registry& registry);
};