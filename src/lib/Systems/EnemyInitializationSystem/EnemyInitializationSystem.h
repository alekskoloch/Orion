#pragma once

#include <entt/entt.hpp>
#include <SFML/System.hpp>

#include "TextureManager.h"

#include "ProceduralGenerationSystem.h"

#include "GraphicsOperations.h"

#include "../entities/EnemyBuilder.h"

#include "WeaponsSystem.h"

#include "WeaponsSchema.h"

#include "enemyModificator.h"

class EnemyInitializationSystem
{
public:
    static void createNewEnemy(entt::registry& registry, sf::Vector2f position);
    static void loadEnemyFromConfig(entt::registry& registry, std::ifstream& configFile, sf::Vector2f position);

    static entt::entity spawnEnemy(entt::registry& registry, sf::Vector2f position, std::string enemyType, std::vector<Modificator> modificators = { Modificator::None } );

private:
    static void drawEnemyModifiers(entt::registry& registry, entt::entity enemy);
};