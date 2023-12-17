#include "BackgroundManager.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../systems/EnemyInitializationSystem.h"
#include "../systems/ProceduralGenerationSystem.h"

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
                sf::Color(100 + (x * 2), 100 + (y * 2), 100)
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
    int minX = playerTileX - tilesAroundPlayer;
    int maxX = playerTileX + tilesAroundPlayer;
    int minY = playerTileY - tilesAroundPlayer;
    int maxY = playerTileY + tilesAroundPlayer;

    std::vector<BackgroundTile> newBackgroundTiles;

    for (const BackgroundTile& tile : backgroundTiles)
    {
        if (tile.position.x >= minX * backgroundTileSize && tile.position.x <= maxX * backgroundTileSize &&
            tile.position.y >= minY * backgroundTileSize && tile.position.y <= maxY * backgroundTileSize)
        {
            newBackgroundTiles.push_back(tile);
        }
    }

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            bool tileExists = false;
            for (const BackgroundTile& tile : newBackgroundTiles)
            {
                sf::Vector2f tilePosition = tile.position;
                if (tilePosition.x == x * backgroundTileSize && tilePosition.y == y * backgroundTileSize)
                {
                    tileExists = true;
                    break;
                }
            }

            if (!tileExists)
            {
                //TODO: Temporary solution for enemy spawning, this should be handled by a system
                if (ProceduralGenerationSystem::GetRandomNumber(1, 10) == 1)
                    EnemyInitializationSystem::createNewEnemy(registry, sf::Vector2f(x * backgroundTileSize, y * backgroundTileSize));

                newBackgroundTiles.push_back(BackgroundTile(
                    sf::Vector2f(x * backgroundTileSize, y * backgroundTileSize),
                    sf::Vector2f(backgroundTileSize, backgroundTileSize),
                    sf::Color(100 + (x * 2), 100 + (y * 2), 100)
                ));
            }
        }
    }

    backgroundTiles = std::move(newBackgroundTiles);
}