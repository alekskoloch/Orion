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
        switch (ProceduralGenerationSystem::GetRandomNumber(1,9))
        {
        case 1:
            createEnemy(registry, enemy);
            break;
        case 2:
            createEnemy(registry, enemy2);
            break;
        case 3:
            createEnemy(registry, enemy3);
            break;
        case 4:
            createEnemy(registry, enemy4);
            break;
        case 5:
            createEnemy(registry, enemyRed1);
            break;
        case 6:
            createEnemy(registry, enemyRed2);
            break;
        case 7:
            createEnemy(registry, enemyRed3);
            break;
        case 8:
            createEnemy(registry, enemyRed4);
            break;
        case 9:
            createEnemy(registry, enemyZombie1);
            break;
        case 10:
            createEnemy(registry, enemyZombie2);
            break;
        case 11:
            createEnemy(registry, enemyZombie3);
            break;
        case 12:
            createEnemy(registry, enemyZombie4);
            break;
        case 13:
            createEnemy(registry, enemyZombie5);
            break;
        default:
            break;
        }
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
    EnemySchema enemySchema = enemy;
    switch (ProceduralGenerationSystem::GetRandomNumber(1, 13))
    {
        case 1:
            enemySchema = enemy;
            break;
        case 2:
            enemySchema = enemy2;
            break;
        case 3:
            enemySchema = enemy3;
            break;
        case 4:
            enemySchema = enemy4;
            break;
        case 5:
            enemySchema = enemyRed1;
            break;
        case 6:
            enemySchema = enemyRed2;
            break;
        case 7:
            enemySchema = enemyRed3;
            break;
        case 8:
            enemySchema = enemyRed4;
            break;
        case 9:
            enemySchema = enemyZombie1;
            break;
        case 10:
            enemySchema = enemyZombie2;
            break;
        case 11:
            enemySchema = enemyZombie3;
            break;
        case 12:
            enemySchema = enemyZombie4;
            break;
        case 13:
            enemySchema = enemyZombie5;
            break;
        default:
            break;
    }

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

    registry.emplace<Position>(enemy, position);
    registry.emplace<Speed>(enemy, enemySchema.speed);
    registry.emplace<Velocity>(enemy, enemySchema.velocity);
    registry.emplace<Name>(enemy, enemySchema.name);
    registry.emplace<Health>(enemy, enemySchema.health, enemySchema.health);

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