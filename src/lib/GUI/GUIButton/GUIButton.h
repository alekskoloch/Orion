#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

#include "FontManager.h"
#include "InputContext.hpp"

class Window;

enum class ButtonStyle
{
    Bordered,
    Borderless
};

class GUIButton
{
public:
    GUIButton() = default;

    GUIButton( const sf::Vector2f& position, const sf::Vector2f& size, const std::string& textString, unsigned int maxFontSize,
               const sf::Color& normalColor, const sf::Color& hoverColor, const sf::Color& activeColor,
               ButtonStyle style = ButtonStyle::Bordered );

    void setPosition( const sf::Vector2f& position );
    void setSize( const sf::Vector2f& size );

    sf::FloatRect getGlobalBounds() const;
    const sf::Vector2f& getPosition() const;

    void setText( const std::string& textString );
    void setOnClick( std::function< void() > onClick );
    void setDefaultState();
    void setSelectedState( bool selectedState = true );

    void handleEvent( const InputContext& inputContext );
    void update( sf::Time deltaTime );

    void draw( Window& window ) const;

private:
    sf::RectangleShape m_shape;

    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    sf::Color selectedColor = sf::Color( 128, 128, 128, 200 );
    ButtonStyle style;
    unsigned int maxFontSize;
    std::function< void() > onClick;

    bool m_isHovered{ false };
    bool m_isPressed{ false };

    bool selectedState = false;
    void adjustTextSize();
};