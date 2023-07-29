#include "EnemyInitializationSystem.h"

#include "../managers/TextureManager.h"
#include "../components/components.h"
#include "../components/tagComponents.h"

//TODO: Only for testing
static float enemyStartPositionX = 1820.f;
static float enemyStartPositionY = 980.f;

void EnemyInitializationSystem::initializeEnemy(entt::registry& registry)
{
    TextureManager::getInstance().loadTexture("ENEMY_TEXTURE", ASSETS_PATH + std::string("enemy.png"));

    auto enemy = registry.create();
    registry.emplace<Enemy>(enemy);
    
    sf::Sprite sprite(TextureManager::getInstance().getTexture("ENEMY_TEXTURE"));
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    registry.emplace<Renderable>(enemy, sprite);
    registry.emplace<Position>(enemy, sf::Vector2f(enemyStartPositionX, enemyStartPositionY));
}