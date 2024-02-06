#pragma once

#include <entt/entt.hpp>
#include <SFML/System.hpp>

#include "velocity.h"
#include "waypointMovement.h"
#include "position.h"
#include "speed.h"

#include "MathOperations.h"

class WaypointsMovementSystem
{
public:
    static void updateWaypoints(entt::registry& registry, sf::Time deltaTime);
};
