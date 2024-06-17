#include "pch.h"
#include "EnemyInitializationSystem.h"

//TODO: Temporary solution for initialize enemy, refactor later
void EnemyInitializationSystem::createNewEnemy(entt::registry& registry, sf::Vector2f position)
{
    std::ifstream configFile(CONFIG_PATH + std::string("enemyConfig.json"));
    if (!configFile.is_open())
        throw std::runtime_error("Could not open config file");
    else
        EnemyInitializationSystem::loadEnemyFromConfig(registry, configFile, position);
}

void EnemyInitializationSystem::loadEnemyFromConfig(entt::registry& registry, std::ifstream& configFile, sf::Vector2f position)
{
    nlohmann::json configJson;
    configFile >> configJson;

    if (!configJson.contains("enemies") || !configJson["enemies"].is_array())
        throw std::runtime_error("Could not find enemies array in config file");

    unsigned int enemiesCount = configJson["enemies"].size();
    if (enemiesCount < 0)
        throw std::runtime_error("Enemies array is empty");

    auto enemyIndex = ProceduralGenerationSystem::GetRandomNumber(2u, enemiesCount - 1);
    const auto& enemyJson = configJson["enemies"][enemyIndex];

    EnemyBuilder enemyBuilder(registry);

    auto newEnemy = enemyBuilder.createEnemy(enemyJson["name"])
                .addEntityState(enemyJson["attackRange"], enemyJson["idleRange"])
                .addPosition(position)
                .addSpeed(enemyJson["speed"])
                .addHealth(enemyJson["health"])
                .addExperience(enemyJson["experience"])
                .addDrop(100, 69)
                .setWeapon(enemyJson["weapon"])
                .setWaypointMovement()
                .build();

    EnemyInitializationSystem::drawEnemyModifiers(registry, newEnemy);
}

entt::entity EnemyInitializationSystem::spawnEnemy(entt::registry& registry, sf::Vector2f position, std::string enemyType, std::vector<Modificator> modificators)
{
    std::ifstream configFile(CONFIG_PATH + std::string("enemyConfig.json"));
    if (!configFile.is_open())
        throw std::runtime_error("Could not open config file");

    nlohmann::json configJson;
    configFile >> configJson;

    if (!configJson.contains("enemies") || !configJson["enemies"].is_array())
        throw std::runtime_error("Could not find enemies array in config file");

    bool enemyFound = false;

    for (const auto& enemyJson : configJson["enemies"])
    {
        if (enemyJson["name"] == enemyType)
        {
            EnemyBuilder enemyBuilder(registry);

            enemyBuilder.createEnemy(enemyJson["name"])
                        .addEntityState(enemyJson["attackRange"], enemyJson["idleRange"])
                        .addPosition(position)
                        .addSpeed(enemyJson["speed"])
                        .addHealth(enemyJson["health"])
                        .addExperience(enemyJson["experience"])
                        .addDrop(100, 69)
                        .setWeapon(enemyJson["weapon"])
                        .setWaypointMovement();

            auto newEnemy = enemyBuilder.build();

            if (!modificators.empty())
            {
                auto& enemyModificator = registry.emplace<EnemyModificator>(newEnemy);
                for (auto& modificator : modificators)
                {
                    enemyModificator.modificators.push_back(modificator);
                }
            }

            enemyFound = true;
            return newEnemy;
        }
    }

    if (!enemyFound)
    {
        throw std::runtime_error("Could not find enemy with name: " + enemyType);
    }

    return entt::entity{};
}

void EnemyInitializationSystem::drawEnemyModifiers(entt::registry& registry, entt::entity enemy)
{
    //todo: add more modifiers

    if (ProceduralGenerationSystem::GetRandomNumber(0, 100) < 5)
        registry.emplace<EnemyModificator>(enemy, std::vector<Modificator>{Modificator::AllySummon});
}