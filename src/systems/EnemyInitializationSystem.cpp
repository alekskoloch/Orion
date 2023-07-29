#include "EnemyInitializationSystem.h"

#include "../managers/TextureManager.h"
#include "../components/components.h"
#include "../components/tagComponents.h"

//TODO: Only for testing
static float enemyStartPositionX = 100.f;
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
    registry.emplace<Speed>(enemy, 600.f);
    registry.emplace<Velocity>(enemy, sf::Vector2f(0.f, 0.f));

    std::vector<sf::Vector2f> waypoints = {
    sf::Vector2f(100.f, 100.f),
    sf::Vector2f(1820.f, 100.f),
    sf::Vector2f(1820.f, 980.f),
    sf::Vector2f(100.f, 980.f)
    };

    registry.emplace<WaypointMovement>(enemy, waypoints);
}