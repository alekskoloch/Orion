#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class RotateTowardsMouseSystem
{
public:
    static void rotateTowardsMouse(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window);
};