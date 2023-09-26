#pragma once

#include "../pch.h"

#include "../managers/TextureManager.h"

#include "../systems/ProceduralGenerationSystem.h"

class BackgroundTile
{
public:
    BackgroundTile(sf::Vector2f position, sf::Vector2f size, sf::Color color)
    {
        //TODO: Temporary solution for loading background object textures
        TextureManager::getInstance().loadTexture("STAR1_TEXTURE", ASSETS_PATH + std::string("backgroundElements/star1.png"));
        TextureManager::getInstance().loadTexture("STAR2_TEXTURE", ASSETS_PATH + std::string("backgroundElements/star2.png"));
        TextureManager::getInstance().loadTexture("STAR3_TEXTURE", ASSETS_PATH + std::string("backgroundElements/star3.png"));

        this->position = position;
        this->size = size;

        tile.setSize(size);
        tile.setPosition(position);

        sf::Color backgroundTileColor = color;
        backgroundTileColor.a = 30;

        tile.setFillColor(backgroundTileColor);

        tile.setOrigin(size.x / 2, size.y / 2);

        //TODO: Temporary, need to find a better way to procedurally generate stars and object
        unsigned int numberOfStars = ProceduralGenerationSystem::GetRandomNumber(0, 4, static_cast<int>(position.x), static_cast<int>(position.y));

        for (int i = 0; i < numberOfStars; i++)
        {
            sf::CircleShape star(ProceduralGenerationSystem::GetRandomNumber(2.f, 4.f, static_cast<int>(position.x) + i, static_cast<int>(position.y) + numberOfStars + i));
            star.setFillColor(color);
            star.setPosition(
                ProceduralGenerationSystem::GetRandomNumber(position.x - size.x / 2, position.x + size.x / 2, static_cast<int>(position.x) + i, static_cast<int>(position.y)),
                ProceduralGenerationSystem::GetRandomNumber(position.y - size.y / 2, position.y + size.y / 2, static_cast<int>(position.y) + i + numberOfStars, static_cast<int>(position.x))
            );

            this->stars.push_back(star);
        }

        int hasObject = ProceduralGenerationSystem::GetRandomNumber(0, 20, static_cast<int>(position.x), static_cast<int>(position.y));

        if (hasObject < 2)
        {
            int objectTexture = ProceduralGenerationSystem::GetRandomNumber(1, 3, hasObject, static_cast<int>(position.x) + static_cast<int>(position.y));

            sf::Sprite object;

            if (objectTexture == 1)
            {
                object.setTexture(TextureManager::getInstance().getTexture("STAR1_TEXTURE"));
            }
            else if (objectTexture == 2)
            {
                object.setTexture(TextureManager::getInstance().getTexture("STAR2_TEXTURE"));
            }
            else if (objectTexture == 3)
            {
                object.setTexture(TextureManager::getInstance().getTexture("STAR3_TEXTURE"));
            }

            object.setScale(0.5f, 0.5f);
            object.setPosition(
                ProceduralGenerationSystem::GetRandomNumber(position.x - size.x / 2, position.x + size.x / 2, static_cast<int>(position.x), static_cast<int>(position.y)),
                ProceduralGenerationSystem::GetRandomNumber(position.y - size.y / 2, position.y + size.y / 2, static_cast<int>(position.y), static_cast<int>(position.x))
            );

            this->objects.push_back(object);
        }
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(tile);

        for (auto& star : stars)
        {
            window.draw(star);
        }

        for (auto& object : objects)
        {
            window.draw(object);
        }
    }

    std::vector<sf::CircleShape> stars;
    std::vector<sf::Sprite> objects;

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

    float backgroundTileSize = 800.f;
    std::vector<BackgroundTile> backgroundTiles;

    void updateBackgroundTiles(int playerTileX, int playerTileY);
};