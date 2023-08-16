#pragma once

#include "../../pch.h"

class GUIEnergyBar
{
public:
    GUIEnergyBar(sf::RenderWindow& window, entt::registry& registry);

    void update();
    void draw();

private:
    sf::RenderWindow& window;
    entt::registry& registry;

    sf::RectangleShape energyBarSprite;
    sf::RectangleShape energyBarBackgroundSprite;

    void initializeEnergyBar();
};