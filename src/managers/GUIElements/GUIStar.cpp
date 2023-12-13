#include "GUIStar.h"

GUIStar::GUIStar(sf::Vector2f position, sf::Color color, float radius, bool flicker, float idleTime, float flickerTime)
    : position(position), color(color), flicker(flicker), idleTime(idleTime), flickerIdleTime(idleTime), flickerTime(flickerTime), exists(false)
{
    this->star.setRadius(radius);
    this->star.setOrigin(radius, radius);
    this->star.setPosition(this->position);
    this->color.a = 0;
    this->star.setFillColor(this->color);
}

void GUIStar::update(float deltaTime)
{
    if (!this->exists)
    {
        this->alphaChannel = std::clamp(this->alphaChannel + MaxAlpha * (deltaTime / this->flickerTime), 0.f, static_cast<float>(MaxAlpha));
        this->color.a = static_cast<sf::Uint8>(this->alphaChannel);
        this->star.setFillColor(this->color);

        if (this->alphaChannel >= MaxAlpha)
            this->exists = true;
    }
    else if (this->flicker)
    {
        if (this->flickerIdleTime > 0.f)
            this->flickerIdleTime -= deltaTime;
        else
            this->handleFlicker(deltaTime);
    }
}

void GUIStar::handleFlicker(float deltaTime)
{
    float alphaDelta = MaxAlpha * (deltaTime / this->flickerTime);
    this->alphaChannel = std::clamp(this->alphaChannel + (this->isDarker ? -alphaDelta : alphaDelta), 0.f, static_cast<float>(MaxAlpha));
    this->color.a = static_cast<sf::Uint8>(this->alphaChannel);
    this->star.setFillColor(this->color);

    if (this->alphaChannel <= 0.f || this->alphaChannel >= MaxAlpha)
        this->isDarker = !this->isDarker;
}

void GUIStar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->star, states);
}

GUIStar GUIStar::getStar()
{
    GUIStar star(this->position, this->color, this->star.getRadius(), this->flicker, this->idleTime);
    return star;
}