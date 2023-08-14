#include "EnemyInitializationSystem.h"

#include "ProceduralGenerationSystem.h"

#include "../managers/TextureManager.h"
#include "../systems/WeaponsSystem.h"
#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../schema/WeaponsSchema.h"

void EnemyInitializationSystem::initializeEnemy(entt::registry& registry)
{
    EnemyInitializationSystem::createEnemy(registry, enemy);
    EnemyInitializationSystem::createEnemy(registry, enemy2);
    EnemyInitializationSystem::createEnemy(registry, enemy3);
    EnemyInitializationSystem::createEnemy(registry, enemy4);
}

void EnemyInitializationSystem::createEnemy(entt::registry& registry, const EnemySchema& enemySchema)
{
    TextureManager::getInstance().loadTexture(enemySchema.textureName, ASSETS_PATH + enemySchema.textureName + ".png");
    TextureManager::getInstance().loadTexture("enemy_bullet", ASSETS_PATH + std::string("enemy_bullet") + ".png");

    auto enemy = registry.create();
    registry.emplace<Enemy>(enemy);

    sf::Sprite sprite(TextureManager::getInstance().getTexture(enemySchema.textureName));
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    registry.emplace<Collision>(enemy, sprite.getGlobalBounds());
    registry.emplace<Renderable>(enemy, sprite);
    registry.emplace<Position>(enemy, sf::Vector2f(ProceduralGenerationSystem::GetRandomNumber<float>(-1920.f, 1920.f), ProceduralGenerationSystem::GetRandomNumber<float>(-1080.f, 1080.f)));
    registry.emplace<Speed>(enemy, enemySchema.speed);
    registry.emplace<Velocity>(enemy, enemySchema.velocity);
    registry.emplace<Name>(enemy, enemySchema.name);

    WeaponsSystem::loadWeapon(registry, enemySchema.weaponSchema, enemy);

    registry.emplace<WaypointMovement>(enemy, enemySchema.waypoints);
}