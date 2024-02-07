#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "QuestSystem.h"

#include "renderable.h"
#include "position.h"
#include "player.h"
#include "enemy.h"

#include "GraphicsOperations.h"

class GUIMinimap
{
public:
    GUIMinimap(sf::RenderWindow& window, entt::registry& registry, std::vector<Quest>& quests);

    void update();
    void draw();
private:
    entt::registry& registry;
    sf::RenderWindow& window;
    std::vector<Quest>& quests;

    sf::CircleShape backgroundMap;
    sf::Sprite playerMinimapSprite;
    sf::Sprite activeQuestMinimapSprite;
    bool drawQuestMarker = false;
    std::vector<sf::CircleShape> mapObjects;
    void updateQuestMarker();

    void initializationMinimap();

    void initializePlayerCoordinatesText();
    void updatePlayerCoordinates();
    void writePlayerCoordinates();
    sf::Font& font;
    sf::Text playerCoordinatesText;

    sf::Text activeQuestTitleText;
    sf::Text activeQuestDescriptionText;
    sf::Text activeQuestDistanceText;

    void initializeActiveQuestText();
    void updateActiveQuestText();
    void drawActiveQuestText();
};