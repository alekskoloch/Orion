#pragma once

#include "../pch.h"

class CollisionSystem
{
public:
    static void updateCollisionBoxes(entt::registry& registry);
    static void checkCollisions(entt::registry& registry);
};