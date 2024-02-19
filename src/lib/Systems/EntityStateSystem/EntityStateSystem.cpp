#include "pch.h"
#include "EntityStateSystem.h"

void EntityStateSystem::updateEntityState(entt::registry& registry, sf::Time& deltaTime)
{
    auto enemies = registry.view<Enemy, Position, EntityState>();
    auto playerPosition = registry.view<Player, Position>().get<Position>(registry.view<Player>().front());
    
    for (auto& enemy : enemies)
    {
        Position& enemyPosition = enemies.get<Position>(enemy);
        
        float distance = CalculateDistance(enemyPosition.position, playerPosition.position);
        auto& enemyState = enemies.get<EntityState>(enemy);
        
        if (distance < enemyState.attackRange)
        {
            enemyState.currentState = EntityState::State::Attacking;
            enemyState.rushTimer = 0.0f;
        }
        else if (distance < enemyState.idleRange)
        {
            if (enemyState.currentState == EntityState::State::Attacking)
            {
                enemyState.rushTimer = 0.0f;
            }
        }
        else
        {
            enemyState.rushTimer += deltaTime.asSeconds();
            if (enemyState.rushTimer >= enemyState.rushDuration)
            {
                enemyState.currentState = EntityState::State::Idle;
                enemyState.rushTimer = 0.0f;
            }
        }
    }
}