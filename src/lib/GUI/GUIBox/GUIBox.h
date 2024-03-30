#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "ConfigManager.hpp"

class GUIBox : public sf::Drawable
{
public:
    GUIBox(float width, float height, sf::Vector2f position, sf::Font& font, sf::Color fillColor = sf::Color(0, 0, 0, 200), sf::Color outlineColor = sf::Color(255, 255, 255, 255), float outlineThickness = 2.f, float bevel = 20);

    void update();

    void addText(std::string text);
    void clearTexts() { this->texts.clear(); }
private:
    sf::Vector2f position;
    float width;
    float height;

    sf::ConvexShape box;
    void initializeBox(float width, float height, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColor, float outlineThickness, float bevel);

    sf::Font& font;
    std::vector<std::unique_ptr<sf::Text>> texts;
    bool textUpdated = false;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};