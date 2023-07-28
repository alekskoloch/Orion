#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class AccelerationSystem
{
public:
    static void accelerate(entt::registry& registry, sf::Time deltaTime);
};