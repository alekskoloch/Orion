#include "GUIBox.h"

GUIBox::GUIBox(float width, float height, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColor, float outlineThickness, float bevel)
{
    this->initializeBox(width, height, position, fillColor, outlineColor, outlineThickness, bevel);
}

void GUIBox::initializeBox(float width, float height, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColor, float outlineThickness, float bevel)
{
    this->box.setPointCount(5);
    this->box.setPoint(0, position);
    this->box.setPoint(1, sf::Vector2f(position.x + width, position.y));
    this->box.setPoint(2, sf::Vector2f(position.x + width, position.y + height - bevel));
    this->box.setPoint(3, sf::Vector2f(position.x + width - bevel, position.y + height));
    this->box.setPoint(4, sf::Vector2f(position.x, position.y + height));

    this->box.setFillColor(fillColor);
    this->box.setOutlineColor(outlineColor);
    this->box.setOutlineThickness(outlineThickness);

    this->box.setOrigin(width / 2.f, height / 2.f);
}

void GUIBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->box, states);
}