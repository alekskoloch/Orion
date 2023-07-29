#include "WaypointsMovementSystem.h"

#include "../components/components.h"

void WaypointsMovementSystem::updateWaypoints(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Velocity, WaypointMovement, Position, Speed>();
    for (auto entity : view) {
        auto& velocity = view.get<Velocity>(entity);
        auto& waypointMovement = view.get<WaypointMovement>(entity);
        auto& position = view.get<Position>(entity);
        auto& speed = view.get<Speed>(entity);

        if (waypointMovement.currentWaypointIndex < waypointMovement.waypoints.size()) {
            sf::Vector2f direction = waypointMovement.waypoints[waypointMovement.currentWaypointIndex] - position.position;
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction /= distance;
            velocity.velocity = direction * speed.maxSpeedValue;

            if (distance < 5.f) {
                waypointMovement.currentWaypointIndex = (waypointMovement.currentWaypointIndex + 1) % waypointMovement.waypoints.size();
            }
        }
    }
}