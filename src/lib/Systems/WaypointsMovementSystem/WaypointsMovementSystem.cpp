#include "WaypointsMovementSystem.h"

const float DISTANCE_TOLERANCE = 5.f;

void WaypointsMovementSystem::updateWaypoints(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Velocity, WaypointMovement, Position, Speed>();
    
    for (auto entity : view)
    {
        auto& entityVelocity = view.get<Velocity>(entity).velocity;
        auto& waypointMovementComponent = view.get<WaypointMovement>(entity);
        auto& entityPosition = view.get<Position>(entity).position;
        auto& speed = view.get<Speed>(entity);

        if (waypointMovementComponent.currentWaypointIndex < waypointMovementComponent.waypoints.size())
        {
            sf::Vector2f direction = CalculateDirectionBetweenPoints(entityPosition, waypointMovementComponent.waypoints[waypointMovementComponent.currentWaypointIndex]);
            entityVelocity = direction * speed.maxSpeedValue;

            float distance = CalculateDistance(entityPosition, waypointMovementComponent.waypoints[waypointMovementComponent.currentWaypointIndex]);
            if (distance < DISTANCE_TOLERANCE)
                waypointMovementComponent.currentWaypointIndex = (waypointMovementComponent.currentWaypointIndex + 1) % waypointMovementComponent.waypoints.size();
        }
    }
}