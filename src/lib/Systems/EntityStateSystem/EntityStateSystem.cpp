#include "pch.h"
#include "EntityStateSystem.h"

void EntityStateSystem::updateEntityState(entt::registry& registry, sf::Time& deltaTime)
{
    auto enemies = registry.view<Enemy, Position, EntityState>();
    auto playerPosition = registry.view<Player, Position>().get<Position>(registry.view<Player>().front()).position;

    for (auto& enemy : enemies)
    {
        auto& enemyPosition = enemies.get<Position>(enemy).position;
        auto& enemyState = enemies.get<EntityState>(enemy);

        float distance = CalculateDistance(enemyPosition, playerPosition);

        if (distance < enemyState.attackRange)
        {
            if (!enemyState.stateMachine->state_cast<const Attacking*>())
            {
                enemyState.stateMachine->process_event(EventAttack());
            }
        }
        else if (distance > enemyState.idleRange)
        {
            if (enemyState.stateMachine->state_cast<const Attacking*>())
            {
                enemyState.stateMachine->process_event(EventRush(deltaTime));
            }
        }
    }
}