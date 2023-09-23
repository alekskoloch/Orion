#include "GUIStar.h"

GUIStar::GUIStar(sf::Vector2f position, sf::Color color, float radius, bool flicker, float idleTime)
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
    this->flickerTime = 5.f;

    this->exists = false;
}

void GUIStar::update(float deltaTime)
{
    if (!this->exists)
    {
        if (this->color.a < 255)
        {
            //TODO: adjust value with time
            this->color.a += 200 * deltaTime;
            if (this->color.a > 255)
                this->color.a = 255;
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
                        this->flickerIdleTime = this->idleTime;
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

GUIStar GUIStar::getStar()
{
    GUIStar star(this->position, this->color, this->star.getRadius(), this->flicker, this->idleTime);
    return star;
}