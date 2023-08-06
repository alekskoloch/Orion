#pragma once

#include "../pch.h"

class WaypointsMovementSystem
{
public:
    static void updateWaypoints(entt::registry& registry, sf::Time deltaTime);
};
