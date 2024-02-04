#pragma once

#include "../../pch.h"

#include "../../managers/FontManager.h"

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

    void update(const sf::Vector2f& mousePos, bool& mouseReleased);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    ButtonStyle style;
    unsigned int maxFontSize;
    std::function<void()> onClick;

    void adjustTextSize();
};