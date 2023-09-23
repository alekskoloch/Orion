#pragma once

#include "../../pch.h"

class GUIStar : public sf::Drawable
{
public:
    GUIStar(sf::Vector2f position, sf::Color color, float radius, bool flicker = false);

    void update(float deltaTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Vector2f position;
    sf::CircleShape star;
    sf::Color color;

    float appearTime = 2.f;
    bool exists = true;

    bool flicker;
    float flickerIdleTime;
    float flickerTime;
    bool isDarker = true;
};