#pragma once

#include <entt/entt.hpp>

#include "../schema/EnemySchema.h"

class EnemyInitializationSystem
{
public:
    static void initializeEnemy(entt::registry& registry);
    static void createEnemy(entt::registry& registry, const EnemySchema& enemySchema);
};