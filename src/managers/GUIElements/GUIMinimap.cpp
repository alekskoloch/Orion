#include "GUIMinimap.h"

#include "../TextureManager.h"

#include "../../components/components.h"
#include "../../components/tagComponents.h"

#include "../../utils/GraphicsOperations.h"

GUIMinimap::GUIMinimap(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry)
{
    initializationMinimap();
}

void GUIMinimap::update()
{
    //TODO: temporary solution, refactor this
    auto player = registry.view<Player, Renderable>().front();
    auto playerPosition = registry.get<Position>(player);
    auto playerRotation = registry.get<Renderable>(player).sprite.getRotation();
    playerMinimapSprite.setRotation(playerRotation);

    mapObjects.clear();

    float maxDistance = backgroundMap.getRadius() * 20.0f;

    auto enemies = registry.view<Enemy, Position>();
    for (auto enemy : enemies)
    {
        auto enemyPosition = registry.get<Position>(enemy);

        float distance = std::sqrt(
            (enemyPosition.position.x - playerPosition.position.x) * (enemyPosition.position.x - playerPosition.position.x) +
            (enemyPosition.position.y - playerPosition.position.y) * (enemyPosition.position.y - playerPosition.position.y)
        );

        if (distance <= maxDistance)
        {
            sf::Vector2f playerMinimapPosition(
                backgroundMap.getPosition().x + backgroundMap.getRadius(),
                backgroundMap.getPosition().y + backgroundMap.getRadius()
            );

            sf::Vector2f enemyMinimapPosition(
                playerMinimapPosition.x + (enemyPosition.position.x - playerPosition.position.x) / 20,
                playerMinimapPosition.y + (enemyPosition.position.y - playerPosition.position.y) / 20
            );

            sf::CircleShape enemyDot;
            enemyDot.setRadius(5);
            enemyDot.setFillColor(sf::Color::Red);
            enemyDot.setPosition(enemyMinimapPosition);
            mapObjects.push_back(enemyDot);
        }
    }
}

void GUIMinimap::draw()
{
    window.draw(backgroundMap);
    window.draw(playerMinimapSprite);
    for (auto mapObject : mapObjects)
        window.draw(mapObject);
}

void GUIMinimap::initializationMinimap()
{
    TextureManager::getInstance().loadTexture("PLAYER_MINIMAP_TEXTURE", ASSETS_PATH + std::string("playerMinimap.png"));

    //TODO: temporary solution, make this configurable
    backgroundMap.setRadius(200);
    backgroundMap.setPointCount(100);
    backgroundMap.setFillColor(sf::Color(128, 128, 128, 200));
    backgroundMap.setOutlineThickness(4);
    backgroundMap.setOutlineColor(sf::Color::White);
    backgroundMap.setPosition(3340, 100);

    playerMinimapSprite = CreateSprite("PLAYER_MINIMAP_TEXTURE");
    playerMinimapSprite.setPosition(backgroundMap.getPosition().x + backgroundMap.getRadius(), backgroundMap.getPosition().y + backgroundMap.getRadius());
}