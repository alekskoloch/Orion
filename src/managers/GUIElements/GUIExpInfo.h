#pragma once

#include "../../pch.h"

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
};