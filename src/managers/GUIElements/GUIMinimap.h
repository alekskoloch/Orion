#pragma once

#include "../../pch.h"

class GUIMinimap
{
public:
    GUIMinimap(sf::RenderWindow& window, entt::registry& registry);

    void update();
    void draw();
private:
    entt::registry& registry;
    sf::RenderWindow& window;

    sf::CircleShape backgroundMap;
    sf::Sprite playerMinimapSprite;
    std::vector<sf::CircleShape> mapObjects;

    void initializationMinimap();
};