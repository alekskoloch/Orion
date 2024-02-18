#pragma once

#include <entt/entt.hpp>
#include <SFML/System.hpp>

#include "velocity.h"
#include "waypointMovement.h"
#include "position.h"
#include "speed.h"
#include "player.h"
#include "enemy.h"
#include "entityState.h"

#include "MathOperations.h"

//TODO: Move to some configuration file
static const float WAYPOINT_GENERATION_DISTANCE_OFFSET = 500.f;

class WaypointsMovementSystem
{
public:
    static void updateWaypoints(entt::registry& registry, sf::Time deltaTime);

private:
    static std::vector<sf::Vector2f> generateWaypointNearPosition(const sf::Vector2f& position);
    static std::vector<sf::Vector2f> generateWaypointsNearPosition(const sf::Vector2f& position);
    static sf::Vector2f generateWaypoint(const sf::Vector2f& position, float distance = WAYPOINT_GENERATION_DISTANCE_OFFSET);

    static void updateAttackingEnemyWaypoints(entt::registry& registry, sf::Time& deltaTime);
};
