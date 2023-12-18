#pragma once

#include "../pch.h"

#include "../schema/EnemySchema.h"

class EnemyInitializationSystem
{
public:
    static void createNewEnemy(entt::registry& registry, sf::Vector2f position);
    static void loadEnemyFromConfig(entt::registry& registry, std::ifstream& configFile, sf::Vector2f position);
};