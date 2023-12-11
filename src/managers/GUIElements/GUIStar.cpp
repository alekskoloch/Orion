#include "GUIStar.h"

GUIStar::GUIStar(sf::Vector2f position, sf::Color color, float radius, bool flicker, float idleTime, float flickerTime)
{
    this->position = position;
    this->color = color;
    this->flicker = flicker;

    this->star.setRadius(radius);
    this->star.setOrigin(radius, radius);
    this->star.setPosition(this->position);
    this->color.a = 0;
    this->star.setFillColor(this->color);

    this->idleTime = idleTime;
    this->flickerIdleTime = idleTime;
    this->flickerTime = flickerTime;

    this->exists = false;
}

void GUIStar::update(float deltaTime)
{
    if (!this->exists)
    {
        if (this->alphaChannel < 255)
        {
            this->alphaChannel += 255 / (this->flickerTime / deltaTime);
            if (this->alphaChannel > 255.f)
                this->alphaChannel = 255.f;
            this->color.a = static_cast<sf::Uint8>(this->alphaChannel);
            this->star.setFillColor(this->color);
        }
        else
        {
            this->exists = true;
        }
    }
    else if (this->flicker)
    {
        if (this->flickerIdleTime > 0.f)
        {
            this->flickerIdleTime -= deltaTime;
        }
        else
        {
            if (this->isDarker)
            {
                if (this->color.a > 0)
                {
                    this->alphaChannel -= 255 / (this->flickerTime / deltaTime);
                    if (this->alphaChannel < 0.f)
                        this->alphaChannel = 0.f;
                    this->color.a = static_cast<sf::Uint8>(this->alphaChannel);
                    this->star.setFillColor(this->color);
                }
                else
                {
                    this->isDarker = false;
                }
            }
            else
            {
                if (this->alphaChannel < 255)
                {
                    this->alphaChannel += 255 / (this->flickerTime / deltaTime);
                    if (this->alphaChannel > 255.f)
                        this->alphaChannel = 255.f;
                    this->color.a = static_cast<sf::Uint8>(this->alphaChannel);
                    this->star.setFillColor(this->color);
                }
                else
                {
                    this->isDarker = true;
                }
            }
        }
    }
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