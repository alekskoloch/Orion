#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

class Window;

class GUIEnergyBar
{
public:
    explicit GUIEnergyBar( entt::registry& registry );

    void update( sf::Time deltaTime );
    void draw( Window& window );

private:
    entt::registry& registry;

    sf::RectangleShape energyBarSprite;
    sf::RectangleShape energyBarBackgroundSprite;

    void initializeEnergyBar();

    bool isNotEnoughEnergy = false;
    float notEnoughEnergyTimer = 1.F;
    bool blink = false;
    float blinkTimer = 0.2F;
};