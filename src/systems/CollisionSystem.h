#pragma once

#include "../pch.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

class CollisionSystem
{
public:
    static void updateCollisionBoxes(entt::registry& registry);
    static void checkCollisions(entt::registry& registry, sf::RenderWindow& window);
};