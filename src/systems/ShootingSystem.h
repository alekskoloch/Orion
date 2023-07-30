#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class ShootingSystem
{
public:
    static void shoot(entt::registry& registry, sf::Time deltaTime);
};