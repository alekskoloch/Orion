#include "GUIButton.h"

GUIButton::GUIButton(const sf::Vector2f& position, const sf::Vector2f& size,
                     const std::string& textString, unsigned int maxFontSize, const sf::Color& normalColor,
                     const sf::Color& hoverColor, const sf::Color& activeColor, ButtonStyle style)
    : normalColor(normalColor), hoverColor(hoverColor), activeColor(activeColor), style(style), maxFontSize(maxFontSize)
{
    this->setOrigin(size.x / 2.f, size.y / 2.f);
    this->setPosition(position);
    this->setSize(size);
    this->setFillColor(normalColor);

    if (style == ButtonStyle::Bordered)
    {
        this->setOutlineThickness(2);
        this->setOutlineColor(sf::Color::White);
    }
    else
    {
        this->setOutlineThickness(0);
    }

    text.setFont(FontManager::getInstance().getFont("font"));
    text.setString(textString);
    text.setFillColor(sf::Color::White);
    adjustTextSize();
}

void GUIButton::setPosition(const sf::Vector2f& position)
{
    sf::RectangleShape::setPosition(position);
    text.setPosition(position);
}

void GUIButton::setText(const std::string& textString)
{
    text.setString(textString);
    adjustTextSize();
}

void GUIButton::setOnClick(std::function<void()> onClick)
{
    this->onClick = onClick;
}

void GUIButton::update(const sf::Vector2f& mousePos, bool& mouseReleased)
{
    if (this->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseReleased)
        {
            this->setFillColor(activeColor);
            mouseReleased = false;
            if (onClick)
            {
                onClick();
            }
        }
        else if (mouseReleased)
        {
            this->setFillColor(hoverColor);
        }
    }
    else if (mouseReleased)
    {
        this->setFillColor(normalColor);
    }
}

void GUIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(static_cast<sf::RectangleShape>(*this), states);
    target.draw(text, states);
}

void GUIButton::adjustTextSize()
{
    auto buttonBounds = this->getGlobalBounds();
    float maxTextWidth = buttonBounds.width - 20.f;

    text.setCharacterSize(maxFontSize);
    while (text.getGlobalBounds().width > maxTextWidth && text.getCharacterSize() > 1)
    {
        text.setCharacterSize(text.getCharacterSize() - 1);
    }

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(this->getPosition());
}