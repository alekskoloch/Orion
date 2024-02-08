#pragma once

#include <entt/entt.hpp>
#include <SFML/System.hpp>

#include "TextureManager.h"

#include "ProceduralGenerationSystem.h"

#include "GraphicsOperations.h"

#include "../entities/EnemyBuilder.h"

#include "WeaponsSystem.h"

#include "WeaponsSchema.h"

class EnemyInitializationSystem
{
public:
    static void createNewEnemy(entt::registry& registry, sf::Vector2f position);
    static void loadEnemyFromConfig(entt::registry& registry, std::ifstream& configFile, sf::Vector2f position);
};