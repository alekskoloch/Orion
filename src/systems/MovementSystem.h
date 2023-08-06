#pragma once

#include "../pch.h"

class MovementSystem
{
public:
    static void updateMovement(entt::registry& registry, sf::Time deltaTime);
};