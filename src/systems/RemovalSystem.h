#pragma once

#include "../pch.h"

class RemovalSystem
{
public:
    static void update(entt::registry& registry);

    static void MarkEntityForDestruction(entt::entity& entity);
private:
    static std::vector<entt::entity> entitiesToDestroy;
    static int framesSinceLastExecution;

    static void checkBulletRemoval(entt::registry& registry);
    static void checkEnemyRemoval(entt::registry& registry);
    static void destroyEntities(entt::registry& registry);
};