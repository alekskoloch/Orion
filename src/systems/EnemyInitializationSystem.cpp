#include "EnemyInitializationSystem.h"

#include "ProceduralGenerationSystem.h"

#include "../entities/EnemyBuilder.h"

#include "../managers/TextureManager.h"
#include "../systems/WeaponsSystem.h"
#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../schema/WeaponsSchema.h"

#include "../utils/GraphicsOperations.h"

//const number of enemies
const int ENEMIES = 6;

void EnemyInitializationSystem::initializeEnemy(entt::registry& registry)
{
    //TODO: Temporary solution for initialize enemy
    for (int i = 0; i < ENEMIES; i++)
    {
        EnemyInitializationSystem::createNewEnemy(registry, sf::Vector2f(i * 100.f + 1000.f, i * 100.f + 1000.f));
    }
}

//TODO: Temporary solution for initialize enemy, refactor later
void EnemyInitializationSystem::createNewEnemy(entt::registry& registry, sf::Vector2f position)
{
    std::string enemyName = "enemy1";

    std::ifstream configFile(CONFIG_PATH + std::string("enemyConfig.json"));
    if (!configFile.is_open())
        throw std::runtime_error("Could not open config file");
    else
        EnemyInitializationSystem::loadEnemyFromConfig(registry, enemyName, configFile, position);
}

void EnemyInitializationSystem::loadEnemyFromConfig(entt::registry& registry, std::string enemyName, std::ifstream& configFile, sf::Vector2f position)
{
    nlohmann::json configJson;
    configFile >> configJson;

    if (!configJson.contains("enemies") || !configJson["enemies"].is_array())
        throw std::runtime_error("Could not find enemies array in config file");
    else
    {
        for (const auto& enemyJson : configJson["enemies"])
        {
            if (enemyJson.contains("name") && enemyJson["name"] == enemyName)
            {
                EnemyBuilder enemyBuilder(registry);

                enemyBuilder.createEnemy(enemyJson["name"])
                            .addEntityState(enemyJson["attackRange"], enemyJson["idleRange"])
                            .addPosition(position)
                            .addSpeed(enemyJson["speed"])
                            .addHealth(enemyJson["health"])
                            .addExperience(enemyJson["experience"])
                            .addDrop(100, 69)
                            .setWeapon(basicEnemyWeapon)
                            .setWaypointMovement();
            }
        }
    }
}