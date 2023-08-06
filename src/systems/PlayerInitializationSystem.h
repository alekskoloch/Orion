#pragma once

#include "../pch.h"

#include "../schema/WeaponsSchema.h"

class PlayerInitializationSystem
{
public:
    static void initializePlayer(entt::registry& registry);
};