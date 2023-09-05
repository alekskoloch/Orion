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

    auto calculateTileIndex = [this](float position)
    {
        float absPosition = std::abs(position);
        return static_cast<int>((absPosition + (backgroundTileSize / 2.0f)) / backgroundTileSize) * (position < 0 ? -1 : 1);
    };

    int playerTileX = calculateTileIndex(playerPosition.position.x);
    int playerTileY = calculateTileIndex(playerPosition.position.y);

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