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

    auto groupEnemyView = registry.view<EnemyGroupLeader>();

    for (auto entity : groupEnemyView)
    {
        if (registry.any_of<EnemyGroupLeader>(entity))
        {
            auto& members = registry.get<EnemyGroupLeader>(entity).members;
            for (auto member : members)
            {
                WaypointsMovementSystem::setWaypointToLeader(registry, member);
            }
        }
    }

    WaypointsMovementSystem::updateAttackingEnemyWaypoints(registry, deltaTime);
}

std::vector<sf::Vector2f> WaypointsMovementSystem::getRandomWaypointsNearPosition(const sf::Vector2f& position)
{
    return generateWaypointsNearPosition(position);
}

void WaypointsMovementSystem::updateAttackingEnemyWaypoints(entt::registry& registry, sf::Time& deltaTime)
{
    auto enemies = registry.view<Enemy, EntityState, WaypointMovement, Position>();

    for (auto& enemy : enemies)
    {
        if (registry.any_of<EnemyGroupMember>(enemy))
            continue;

        auto& enemyEntityState = enemies.get<EntityState>(enemy);
        auto& enemyWaypointMovement = enemies.get<WaypointMovement>(enemy);
        auto playerPosition = registry.view<Position>().get<Position>(registry.view<Player>().front()).position;

        if (enemyEntityState.stateMachine->state_cast<const Attacking*>() != nullptr)
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
        else if (enemyEntityState.stateMachine->state_cast<const Idle*>() != nullptr && enemyWaypointMovement.waypoints.size() == 1)
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

void WaypointsMovementSystem::setWaypointToLeader(entt::registry& registry, entt::entity member)
{
    auto& leader = registry.get<EnemyGroupMember>(member).leader;
    if (registry.valid(leader))
    {
        auto& leaderPosition = registry.view<Position>().get<Position>(leader).position;
        auto& entityOffset = registry.get<EnemyGroupMember>(member).offset;

        registry.view<WaypointMovement>().get<WaypointMovement>(member).waypoints = {sf::Vector2f(leaderPosition.x + entityOffset.x, leaderPosition.y + entityOffset.y)};
    }
}