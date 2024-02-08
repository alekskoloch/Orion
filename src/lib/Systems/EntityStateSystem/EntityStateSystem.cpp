#include "pch.h"
#include "EntityStateSystem.h"

void EntityStateSystem::updateEntityState(entt::registry& registry)
{
    auto enemies = registry.view<Enemy, Position, EntityState>();
    auto playerPosition = registry.view<Player, Position>().get<Position>(registry.view<Player>().front());
    
    for (auto& enemy : enemies)
    {
        Position& enemyPosition = enemies.get<Position>(enemy);
        
        float distance = CalculateDistance(enemyPosition.position, playerPosition.position);
        auto& enemyState = enemies.get<EntityState>(enemy);
        
        if (distance < enemyState.attackRange && enemyState.currentState == EntityState::State::Idle)
            enemyState.currentState = EntityState::State::Attacking;
        else if (distance > enemyState.idleRange && enemyState.currentState == EntityState::State::Attacking)
            enemyState.currentState = EntityState::State::Idle;
    }
}