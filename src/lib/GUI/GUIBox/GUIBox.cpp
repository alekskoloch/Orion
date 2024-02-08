#include "pch.h"
#include "GUIBox.h"

GUIBox::GUIBox(float width, float height, sf::Vector2f position, sf::Font& font, sf::Color fillColor, sf::Color outlineColor, float outlineThickness, float bevel) : font(font)
{
    this->position = position;
    this->width = width;
    this->height = height;

    this->initializeBox(width, height, position, fillColor, outlineColor, outlineThickness, bevel);
}

void GUIBox::update()
{
    if (!this->textUpdated)
    {
        for (int i = 0; i < this->texts.size(); i++)
        {
            this->texts[i]->setOrigin(
                this->texts[i]->getGlobalBounds().width / 2.f,
                this->texts[i]->getGlobalBounds().height / 2.f
            );

            this->texts[i]->setPosition(
                this->position.x,
                this->position.y - this->height / 2 + 40 + i * 40
            );
        }

        this->textUpdated = true;
    }
}

void GUIBox::addText(std::string text)
{
    sf::Text textToAdd;
    textToAdd.setString(text);
    textToAdd.setFont(this->font);
    textToAdd.setCharacterSize(30);
    textToAdd.setFillColor(sf::Color::White);
    textToAdd.setOutlineColor(sf::Color::Black);
    textToAdd.setOutlineThickness(2.f);

    this->texts.push_back(std::make_unique<sf::Text>(textToAdd));

    this->textUpdated = false;
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

    for (auto& text : this->texts)
        target.draw(*text, states);    
}