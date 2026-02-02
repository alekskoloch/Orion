#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class Window;

struct Quest;

class GUIMinimap
{
public:
    GUIMinimap( entt::registry& registry, std::vector< Quest >& quests );

    void update();
    void draw( Window& window );

private:
    entt::registry& registry;
    std::vector< Quest >& quests;

    sf::CircleShape backgroundMap;
    sf::Sprite playerMinimapSprite;
    sf::Sprite activeQuestMinimapSprite;
    bool drawQuestMarker = false;
    std::vector< sf::CircleShape > mapObjects;
    void updateQuestMarker();

    void initializationMinimap();

    void initializePlayerCoordinatesText();
    void updatePlayerCoordinates();
    void writePlayerCoordinates( Window& window );
    sf::Font& font;
    sf::Text playerCoordinatesText;

    sf::Text activeQuestTitleText;
    sf::Text activeQuestDescriptionText;
    sf::Text activeQuestDistanceText;

    void initializeActiveQuestText();
    void updateActiveQuestText();
    void drawActiveQuestText( Window& window );
};