#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include "ConfigManager.hpp"
#include "EventManager.h"

#include "energy.h"
#include "player.h"

class GUIEnergyBar
{
public:
    GUIEnergyBar(sf::RenderWindow& window, entt::registry& registry);

    void update(sf::Time deltaTime);
    void draw();

private:
    sf::RenderWindow& window;
    entt::registry& registry;

    sf::RectangleShape energyBarSprite;
    sf::RectangleShape energyBarBackgroundSprite;

    void initializeEnergyBar();

    bool isNotEnoughEnergy = false;
    float notEnoughEnergyTimer = 1.f;
    bool blink = false;
    float blinkTimer = 0.2f;
};