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

    sf::RectangleShape m_energyBarSprite;
    sf::RectangleShape m_energyBarBackgroundSprite;

    void initializeEnergyBar();

    bool m_isNotEnoughEnergy = false;
    float m_notEnoughEnergyTimer;

    bool m_blink{ false };
    float m_blinkTimer;
};