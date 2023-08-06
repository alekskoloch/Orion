#pragma once

#include "../pch.h"

class RotateTowardsMouseSystem
{
public:
    static void rotateTowardsMouse(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window);
};