#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "ConfigManager.hpp"

#include "experience.h"
#include "player.h"

class GUIExpInfo : public sf::Drawable
{
public:
    GUIExpInfo(sf::RenderWindow& window, entt::registry& registry);

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    sf::RectangleShape backgroundBar;
    sf::RectangleShape expBar;
    bool skillPointsAvailableVisible = false;
    sf::CircleShape skillPointsAvailable;
};