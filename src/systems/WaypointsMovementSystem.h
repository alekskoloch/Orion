#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class WaypointsMovementSystem
{
public:
    static void updateWaypoints(entt::registry& registry, sf::Time deltaTime);
};
