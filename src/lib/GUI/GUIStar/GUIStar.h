#pragma once

#include <SFML/Graphics.hpp>

class GUIStar : public sf::Drawable
{
public:
    explicit GUIStar(sf::Vector2f position, sf::Color color, float radius, bool flicker, float idleTime, float flickerTime = 2.f);

    void update(float deltaTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    GUIStar getStar();
private:
    sf::Vector2f position;
    sf::CircleShape star;
    sf::Color color;

    bool exists;

    bool flicker;
    float idleTime;
    float flickerIdleTime;
    float flickerTime;
    bool isDarker = true;
    float alphaChannel = 0.f;

    static constexpr sf::Uint8 MaxAlpha = 255;

    void handleFlicker(float deltaTime);
};