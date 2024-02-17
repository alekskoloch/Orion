#include "pch.h"
#include "RemovalSystem.h"

std::vector<entt::entity> RemovalSystem::entitiesToDestroy = {};
int RemovalSystem::framesSinceLastExecution = 0;

void RemovalSystem::update(entt::registry& registry)
{
    auto view = registry.view<Removable>();
    for (auto entity : view)
    {
        auto& removable = view.get<Removable>(entity);
        if (removable.remove)
        {
            registry.destroy(entity);
        }
    }

    framesSinceLastExecution++;
    switch (framesSinceLastExecution)
    {
    case 0:
        checkBulletRemoval(registry);
        break;
    case 1:
        checkEnemyRemoval(registry);
        break;
    case 2:
        destroyEntities(registry);
        break;
    default:
        framesSinceLastExecution = -1;
        break;
    }
}

void RemovalSystem::MarkEntityForDestruction(entt::entity& entity)
{
    entitiesToDestroy.push_back(entity);
}

void RemovalSystem::checkBulletRemoval(entt::registry& registry)
{
    auto view = registry.view<Bullet, Position>();
    auto playerView = registry.view<Player, Position>();
    auto playerPosition = playerView.get<Position>(playerView.front()).position;
    for (auto entity : view)
    {
        auto bulletPosition = view.get<Position>(entity);
        if (CalculateDistance(bulletPosition.position, playerPosition) > 4000.f)
            RemovalSystem::MarkEntityForDestruction(entity);
    }
}

void RemovalSystem::checkEnemyRemoval(entt::registry& registry)
{
    auto view = registry.view<Enemy, Position>();
    auto playerView = registry.view<Player, Position>();
    auto playerPosition = playerView.get<Position>(playerView.front()).position;
    for (auto entity : view)
    {
        auto enemyPosition = view.get<Position>(entity);
        if (CalculateDistance(enemyPosition.position, playerPosition) > 5000.f)
            RemovalSystem::MarkEntityForDestruction(entity);
    }
}

void RemovalSystem::destroyEntities(entt::registry& registry)
{
    for (auto entity : entitiesToDestroy)
    {
        if (registry.valid(entity))
            registry.destroy(entity);
    }

    entitiesToDestroy.clear();
}