#include "BackgroundManager.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

//TODO: Color for tiles is only for testing purposes

BackgroundManager::BackgroundManager(entt::registry& registry, sf::RenderWindow& window) : registry(registry), window(window)
{
    for (int x = -tilesAroundPlayer; x < tilesAroundPlayer+1; x++)
    {
        for (int y = -tilesAroundPlayer; y < tilesAroundPlayer+1; y++)
        {
            backgroundTiles.push_back(BackgroundTile(
                sf::Vector2f(0.f + x * backgroundTileSize, 0.f + y * backgroundTileSize),
                sf::Vector2f(backgroundTileSize, backgroundTileSize),
                sf::Color(100 + (x * 10), 100 + (y * 10), 100)
            ));
        }
    }
}

void BackgroundManager::update()
{
    auto playerView = registry.view<Player, Position>();
    auto playerEntity = playerView.front();
    auto& playerPosition = playerView.get<Position>(playerEntity);

    int playerTileX = static_cast<int>(playerPosition.position.x / backgroundTileSize);
    int playerTileY = static_cast<int>(playerPosition.position.y / backgroundTileSize);

    if (playerTileX != currentPlayerTileX || playerTileY != currentPlayerTileY)
    {
        currentPlayerTileX = playerTileX;
        currentPlayerTileY = playerTileY;

        updateBackgroundTiles(playerTileX, playerTileY);
    }
}

void BackgroundManager::draw()
{
    for (auto& backgroundTile : backgroundTiles)
    {
        backgroundTile.draw(window);
    }
}

void BackgroundManager::updateBackgroundTiles(int playerTileX, int playerTileY)
{
    backgroundTiles.clear();

    for (int x = playerTileX - tilesAroundPlayer; x <= playerTileX + tilesAroundPlayer; x++)
    {
        for (int y = playerTileY - tilesAroundPlayer; y <= playerTileY + tilesAroundPlayer; y++)
        {
            backgroundTiles.push_back(BackgroundTile(
                sf::Vector2f(x * backgroundTileSize, y * backgroundTileSize),
                sf::Vector2f(backgroundTileSize, backgroundTileSize),
                sf::Color(100 + (x * 10), 100 + (y * 10), 100)
            ));
        }
    }
}