#pragma once

#include "../../pch.h"

enum class ButtonStyle
{
    Bordered,
    Borderless
};

class GUIButton : public sf::RectangleShape
{
public:
    GUIButton() = default;

    GUIButton(const sf::Vector2f& position, const sf::Vector2f& size,
              const std::string& textString, unsigned int maxFontSize, const sf::Color& normalColor,
              const sf::Color& hoverColor, const sf::Color& activeColor, ButtonStyle style = ButtonStyle::Bordered);

    void setPosition(const sf::Vector2f& position);
    void setText(const std::string& textString);
    void setOnClick(std::function<void()> onClick);
    void setDefaultState();
    //TODO: Color should be configurable
    void setSelectedState(bool selectedState = true);

    void update(const sf::Vector2f& mousePos, bool& mouseReleased);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    sf::Color selectedColor = sf::Color(128, 128, 128, 200);
    ButtonStyle style;
    unsigned int maxFontSize;
    std::function<void()> onClick;

    bool selectedState = false;
    void adjustTextSize();
};