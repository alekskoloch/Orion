#pragma once

#include "../pch.h"

class BackgroundTile
{
public:
    BackgroundTile(sf::Vector2f position, sf::Vector2f size, sf::Color color)
    {
        this->position = position;
        this->size = size;

        tile.setSize(size);
        tile.setPosition(position);
        tile.setFillColor(color);
        
        tile.setOrigin(size.x / 2, size.y / 2);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(tile);
    }

private:
    sf::Vector2f position;
    sf::Vector2f size;

    sf::RectangleShape tile;
};

class BackgroundManager
{
public:
    BackgroundManager(entt::registry& registry, sf::RenderWindow& window);

    void update();
    void draw();
    
private:
    entt::registry& registry;
    sf::RenderWindow& window;

    int currentPlayerTileX = 0;
    int currentPlayerTileY = 0;

    int tilesAroundPlayer = 6;

    float backgroundTileSize = 400.f;
    std::vector<BackgroundTile> backgroundTiles;

    void updateBackgroundTiles(int playerTileX, int playerTileY);
};