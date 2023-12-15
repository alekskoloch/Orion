#include "EnemyInitializationSystem.h"

#include "ProceduralGenerationSystem.h"

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
                auto enemy = registry.create();

                registry.emplace<Enemy>(enemy);
                registry.emplace<EntityState>(enemy);

                auto& enemyState = registry.get<EntityState>(enemy);
                enemyState.attackRange = enemyJson["attackRange"].get<float>();
                enemyState.idleRange = enemyJson["idleRange"].get<float>();

                sf::Sprite sprite = CreateSprite(enemyJson["name"].get<std::string>());
                registry.emplace<Collision>(enemy, sprite.getGlobalBounds());
                registry.emplace<Renderable>(enemy, sprite);

                registry.emplace<Position>(enemy, position);
                registry.emplace<Speed>(enemy, enemyJson["speed"].get<float>());
                registry.emplace<Velocity>(enemy, sf::Vector2f(0.f, 0.f));
                registry.emplace<Name>(enemy, enemyJson["name"].get<std::string>());
                registry.emplace<Health>(enemy, enemyJson["health"].get<float>(), enemyJson["health"].get<float>());

                unsigned int LEVEL = 1;
                registry.emplace<Experience>(enemy, LEVEL, enemyJson["exp"].get<unsigned int>());
                registry.emplace<Drop>(enemy, 100, 69);

                //if enemyJons weapon == basicEnemyWeapon
                if (enemyJson["weapon"] == "basicEnemyWeapon")
                {
                    WeaponsSystem::loadWeapon(registry, basicEnemyWeapon, enemy);
                }
                
                int numOfWaypoints = ProceduralGenerationSystem::GetRandomNumber(2, 6);
                std::vector<sf::Vector2f> waypoints;

                auto enemyPos = registry.get<Position>(enemy);

                for (int i = 0; i < numOfWaypoints; i++)
                {
                    float x = ProceduralGenerationSystem::GetRandomNumber(enemyPos.position.x - 500.f, enemyPos.position.x + 500.f);
                    float y = ProceduralGenerationSystem::GetRandomNumber(enemyPos.position.y - 500.f, enemyPos.position.y + 500.f);

                    waypoints.push_back(sf::Vector2f(x, y));
                }

                registry.emplace<WaypointMovement>(enemy, waypoints);
            }
        }
    }
}