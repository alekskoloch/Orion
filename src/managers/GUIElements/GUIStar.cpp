#include "GUIStar.h"

GUIStar::GUIStar(sf::Vector2f position, sf::Color color, float radius, bool flicker)
{
    this->position = position;
    this->color = color;
    this->flicker = flicker;

    this->star.setRadius(radius);
    this->star.setOrigin(radius, radius);
    this->star.setPosition(this->position);
    color.a = 0;
    this->star.setFillColor(this->color);

    this->flickerIdleTime = 3.f;
    this->flickerTime = 5.f;
}

void GUIStar::update(float deltaTime)
{
    if (!this->exists)
    {
        if (this->color.a < 255)
        {
            this->color.a += 255 / (this->appearTime / deltaTime);
            this->star.setFillColor(this->color);
        }
        else
        {
            this->exists = true;
        }
    }
    else
    {
        if (this->flickerIdleTime > 0.f)
            this->flickerIdleTime -= deltaTime;
        else if (this->flickerIdleTime <= 0.f)
        {
            if (this->isDarker)
            {
                this->flickerTime -= deltaTime;
                if (this->flickerTime <= 0)
                {
                    this->flickerTime = 0;
                    this->isDarker = false;
                }
                else if (this->color.a > 0)
                {
                    //TODO: adjust value with time
                    this->color.a -= 100 * deltaTime;
                    if (this->color.a < 0)
                        this->color.a = 0;
                    this->star.setFillColor(this->color);
                }
            }
            else
            {
                if (this->flickerTime < 5.f)
                {
                    this->flickerTime += deltaTime;
                    if (this->flickerTime >= 5.f)
                    {
                        this->isDarker = true;
                        this->flickerIdleTime = 3.f;
                    }
                    else if (this->color.a < 255)
                    {
                        //TODO: adjust value with time
                        this->color.a += 100 * deltaTime;
                        if (this->color.a > 255)
                            this->color.a = 255;
                        this->star.setFillColor(this->color);
                    }
                }
            }
        }
    }
}

void GUIStar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->star, states);
}