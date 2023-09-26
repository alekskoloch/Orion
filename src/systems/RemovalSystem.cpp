#include "RemovalSystem.h"

#include "../components/Components.h"
#include "../components/tagComponents.h"

#include "../utils/MathOperations.h"

void RemovalSystem::update(entt::registry& registry)
{
    auto view = registry.view<Bullet, Position>();
    auto playerView = registry.view<Player, Position>();
    auto playerPosition = playerView.get<Position>(playerView.front()).position;
    for (auto entity : view)
    {
        auto bulletPosition = view.get<Position>(entity);
        if (CalculateDistance(bulletPosition.position, playerPosition) > 2500.f)
        {
            registry.destroy(entity);
            continue;
        }
    }
}