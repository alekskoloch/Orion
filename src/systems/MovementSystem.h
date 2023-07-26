#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class MovementSystem
{
public:
    static void updateMovement(entt::registry& registry, sf::Time deltaTime);
};