#pragma once

#include "../pch.h"

#include "../systems/ProceduralGenerationSystem.h"

class BackgroundTile
{
public:
    BackgroundTile(sf::Vector2f position, sf::Vector2f size, sf::Color color)
    {
        this->position = position;
        this->size = size;

        tile.setSize(size);
        tile.setPosition(position);

        sf::Color backgroundTileColor = color;
        backgroundTileColor.a = 30;

        tile.setFillColor(backgroundTileColor);

        tile.setOrigin(size.x / 2, size.y / 2);

        unsigned int numberOfStars = ProceduralGenerationSystem::GetRandomNumber(0, 4, static_cast<int>(position.x), static_cast<int>(position.y));

        for (int i = 0; i < numberOfStars; i++)
        {
            sf::CircleShape star(ProceduralGenerationSystem::GetRandomNumber(2.f, 4.f, static_cast<int>(position.x) + i, static_cast<int>(position.y) + numberOfStars + i));
            star.setFillColor(color);
            star.setPosition(
                //TODO: This is temporary, need to find a better way to generate stars
                ProceduralGenerationSystem::GetRandomNumber(position.x - size.x / 2, position.x + size.x / 2, static_cast<int>(position.x) + i, static_cast<int>(position.y)),
                ProceduralGenerationSystem::GetRandomNumber(position.y - size.y / 2, position.y + size.y / 2, static_cast<int>(position.y) + i + numberOfStars, static_cast<int>(position.x))
            );

            this->stars.push_back(star);
        }
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(tile);

        for (auto& star : stars)
        {
            window.draw(star);
        }
    }

    std::vector<sf::CircleShape> stars;

    //TODO: For DebugSystem
    sf::RectangleShape& getTile() { return tile; }

    //TODO: Temporary for optimization in updateBackgroundTiles
    sf::Vector2f position;
private:
    sf::Vector2f size;

    sf::RectangleShape tile;
};

class BackgroundManager
{
public:
    BackgroundManager(entt::registry& registry, sf::RenderWindow& window);

    void update();
    void draw();

    //TODO: For DebugSystem
    std::vector<BackgroundTile>& getBackgroundTiles() { return backgroundTiles; }
private:
    entt::registry& registry;
    sf::RenderWindow& window;

    int currentPlayerTileX = 0;
    int currentPlayerTileY = 0;

    int tilesAroundPlayer = 5;

    float backgroundTileSize = 400.f;
    std::vector<BackgroundTile> backgroundTiles;

    void updateBackgroundTiles(int playerTileX, int playerTileY);
};