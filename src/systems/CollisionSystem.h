#pragma once

#include <entt/entt.hpp>

class CollisionSystem
{
public:
    static void updateCollisionBoxes(entt::registry& registry);
    static void checkCollisions(entt::registry& registry);
};