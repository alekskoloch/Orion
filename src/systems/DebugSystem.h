#pragma once

#include "../pch.h"

class DebugSystem
{
public:
    static void renderCollisionBoxes(entt::registry& registry, sf::RenderWindow& window);
    static void renderAttackRangeCircles(entt::registry& registry, sf::RenderWindow& window);
};