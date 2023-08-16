#include "EntityStateSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

//TODO: Move this to utils and refactor in other systems
float CalculateDistance(const sf::Vector2f& point1, const sf::Vector2f& point2)
{
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;
    
    return std::sqrt(dx * dx + dy * dy);
}

void EntityStateSystem::updateEntityState(entt::registry& registry)
{
    auto enemies = registry.view<Enemy, Position, EntityState>();
    auto playerPosition = registry.view<Player, Position>().get<Position>(registry.view<Player>().front());
    
    for (auto& enemy : enemies)
    {
        Position& enemyPosition = enemies.get<Position>(enemy);
        
        float distance = CalculateDistance(enemyPosition.position, playerPosition.position);
        auto& enemyState = enemies.get<EntityState>(enemy);
        
        if (distance < 1000.f)
            enemyState.currentState = EntityState::State::Attacking;
        else
            enemyState.currentState = EntityState::State::Idle;
    }
}