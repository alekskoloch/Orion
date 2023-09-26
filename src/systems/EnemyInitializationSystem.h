#pragma once

#include "../pch.h"

#include "../schema/EnemySchema.h"

class EnemyInitializationSystem
{
public:
    static void initializeEnemy(entt::registry& registry);
    static void createEnemy(entt::registry& registry, const EnemySchema& enemySchema);
    static void createNewEnemy(entt::registry& registry, sf::Vector2f position);
};