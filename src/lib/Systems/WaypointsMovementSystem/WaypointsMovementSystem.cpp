#include "pch.h"
#include "WaypointsMovementSystem.h"

#include "ProceduralGenerationSystem.h"

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

void WaypointsMovementSystem::updateAttackingEnemyWaypoints(entt::registry& registry, sf::Time& deltaTime)
{
    auto enemies = registry.view<Enemy, EntityState, WaypointMovement, Position>();

    for (auto& enemy : enemies)
    {
        auto& enemyEntityState = enemies.get<EntityState>(enemy);
        auto& enemyWaypointMovement = enemies.get<WaypointMovement>(enemy);
        auto playerPosition = registry.view<Position>().get<Position>(registry.view<Player>().front()).position;

        if (enemyEntityState.currentState == EntityState::State::Attacking)
        {
            if (!enemyEntityState.waypointMovementChanged)
            {
                enemyWaypointMovement.waypoints = generateWaypointNearPosition(playerPosition);
                enemyWaypointMovement.currentWaypointIndex = 0;
                enemyEntityState.waypointMovementChanged = true;
            }
            else
            {
                float distanceToCurrentWaypoint = CalculateDistance(enemies.get<Position>(enemy).position, enemyWaypointMovement.waypoints[enemyWaypointMovement.currentWaypointIndex]);

                if (distanceToCurrentWaypoint < DISTANCE_TOLERANCE)
                    enemyWaypointMovement.waypoints = generateWaypointNearPosition(playerPosition);
            }
        }
        else if (enemyEntityState.currentState == EntityState::State::Idle && enemyWaypointMovement.waypoints.size() == 1)
        {
            enemyWaypointMovement.waypoints = generateWaypointsNearPosition(enemies.get<Position>(enemy).position);
            enemyWaypointMovement.currentWaypointIndex = 0;
            enemyEntityState.waypointMovementChanged = false;
        }
    }
}

std::vector<sf::Vector2f> WaypointsMovementSystem::generateWaypointNearPosition(const sf::Vector2f& position)
{
    return {WaypointsMovementSystem::generateWaypoint(position)};
}

std::vector<sf::Vector2f> WaypointsMovementSystem::generateWaypointsNearPosition(const sf::Vector2f& position)
{
    std::vector<sf::Vector2f> waypoints;
    for (int i = 0; i < ProceduralGenerationSystem::GetRandomNumber(3, 6); i++)
    {
        waypoints.push_back(generateWaypoint(position));
    }
    return waypoints;
}

sf::Vector2f WaypointsMovementSystem::generateWaypoint(const sf::Vector2f& position, float distance)
{
    return sf::Vector2f(
        ProceduralGenerationSystem::GetRandomNumber(position.x - distance, position.x + distance),
        ProceduralGenerationSystem::GetRandomNumber(position.y - distance, position.y + distance)
    );
}