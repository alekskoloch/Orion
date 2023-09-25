#pragma once

#include "../../pch.h"

class GUIBox : public sf::Drawable
{
public:
    GUIBox(float width, float height, sf::Vector2f position, sf::Color fillColor = sf::Color(0, 0, 0, 200), sf::Color outlineColor = sf::Color(255, 255, 255, 255), float outlineThickness = 2.f, float bevel = 20);

private:
    sf::ConvexShape box;
    void initializeBox(float width, float height, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColor, float outlineThickness, float bevel);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};