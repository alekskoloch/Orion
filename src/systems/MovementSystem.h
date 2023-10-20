#pragma once

#include "../pch.h"

class MovementSystem
{
public:
    static void updateMovement(entt::registry& registry, sf::Time deltaTime);

private:
    static void updateMovementBoost(entt::registry& registry, sf::Time deltaTime);
};