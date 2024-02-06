#pragma once

#include <SFML/System.hpp>
#include <entt/entt.hpp>

class AccelerationSystem
{
public:
    static void accelerate(entt::registry& registry, sf::Time deltaTime);
};