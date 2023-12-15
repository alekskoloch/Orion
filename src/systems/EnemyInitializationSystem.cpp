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

void EnemyInitializationSystem::createEnemy(entt::registry& registry, const EnemySchema& enemySchema)
{
    TextureManager::getInstance().loadTexture(enemySchema.textureName, ASSETS_PATH + enemySchema.textureName + ".png");
    TextureManager::getInstance().loadTexture("enemy_bullet", ASSETS_PATH + std::string("enemy_bullet") + ".png");

    auto enemy = registry.create();
    registry.emplace<Enemy>(enemy);
    registry.emplace<EntityState>(enemy);

    //TODO: Temporary solution to set ranges
    auto& enemyState = registry.get<EntityState>(enemy);
    enemyState.attackRange = enemySchema.attackRange;
    enemyState.idleRange = enemySchema.idleRange;

    sf::Sprite sprite = CreateSprite(enemySchema.textureName);
    registry.emplace<Collision>(enemy, sprite.getGlobalBounds());
    registry.emplace<Renderable>(enemy, sprite);

    //TODO: Temporary solution for procedural enemy position
    auto player = registry.view<Player, Position>();
    auto playerPos = registry.get<Position>(*player.begin());

    const float screenWidth = 3840.0f;
    const float screenHeight = 2160.0f;

    float xPos = 0;
    float yPos = 0;

    while (std::abs(xPos) <= (screenWidth / 2) && std::abs(yPos) <= (screenHeight / 2))
    {
        xPos = ProceduralGenerationSystem::GetRandomNumber(-screenWidth, screenWidth);
        yPos = ProceduralGenerationSystem::GetRandomNumber(-screenHeight, screenHeight);
    }

    sf::Vector2f newPosition(playerPos.position.x + xPos, playerPos.position.y + yPos);

    registry.emplace<Position>(enemy, newPosition);
    registry.emplace<Speed>(enemy, enemySchema.speed);
    registry.emplace<Velocity>(enemy, enemySchema.velocity);
    registry.emplace<Name>(enemy, enemySchema.name);
    registry.emplace<Health>(enemy, enemySchema.health, enemySchema.health);

    //TODO: change level value
    unsigned int LEVEL = 1;
    registry.emplace<Experience>(enemy, LEVEL, enemySchema.experience);
    registry.emplace<Drop>(enemy, 100, 69);

    WeaponsSystem::loadWeapon(registry, enemySchema.weaponSchema, enemy);

    //TODO: Temporary solution for waypoints
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