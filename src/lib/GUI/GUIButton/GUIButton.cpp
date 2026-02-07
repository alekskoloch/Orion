#include "GUIButton.h"
#include "Window.hpp"
#include "pch.h"


GUIButton::GUIButton( const sf::Vector2f& position, const sf::Vector2f& size, const std::string& textString,
                      unsigned int maxFontSize, const sf::Color& normalColor, const sf::Color& hoverColor,
                      const sf::Color& activeColor, ButtonStyle style )
    : normalColor( normalColor ), hoverColor( hoverColor ), activeColor( activeColor ), style( style ),
      maxFontSize( maxFontSize ), text{ FontManager::getInstance().getFont( "font" ), textString }
{
    m_shape.setSize( size );
    m_shape.setOrigin( { size.x / 2.f, size.y / 2.f } );
    m_shape.setFillColor( normalColor );

    this->setPosition( position );

    if ( style == ButtonStyle::Bordered )
    {
        m_shape.setOutlineThickness( 2.f );
        m_shape.setOutlineColor( sf::Color::White );
    }
    else
    {
        m_shape.setOutlineThickness( 0.f );
    }

    text.setFillColor( sf::Color::White );
    adjustTextSize();
}

void GUIButton::setPosition( const sf::Vector2f& position )
{
    m_shape.setPosition( position );
    text.setPosition( position );
}

void GUIButton::setSize( const sf::Vector2f& size )
{
    m_shape.setSize( size );
    m_shape.setOrigin( { size.x / 2.f, size.y / 2.f } );
    adjustTextSize();
}

sf::FloatRect GUIButton::getGlobalBounds() const { return m_shape.getGlobalBounds(); }

const sf::Vector2f& GUIButton::getPosition() const { return m_shape.getPosition(); }

void GUIButton::setText( const std::string& textString )
{
    text.setString( textString );
    adjustTextSize();
}

void GUIButton::setOnClick( std::function< void() > onClick ) { this->onClick = onClick; }

void GUIButton::setDefaultState()
{
    this->selectedState = false;
    m_shape.setFillColor( normalColor );
}

void GUIButton::setSelectedState( bool selectedState ) { this->selectedState = selectedState; }

void GUIButton::handleEvent( const InputContext& inputContext )
{
    const auto& event = inputContext.getEvent();
    const auto& mouse = inputContext.getMouseState();

    const auto mousePosition =
        sf::Vector2f{ static_cast< float >( mouse.screenPosition.x ), static_cast< float >( mouse.screenPosition.y ) };

    if ( m_shape.getGlobalBounds().contains( mousePosition ) )
    {
        m_isHovered = true;

        if ( const auto* pressed = event.getIf< sf::Event::MouseButtonPressed >() )
        {
            if ( pressed->button == sf::Mouse::Button::Left )
            {
                m_isPressed = true;
            }
        }
        else if ( const auto* released = event.getIf< sf::Event::MouseButtonReleased >() )
        {
            if ( released->button == sf::Mouse::Button::Left )
            {
                if ( m_isPressed && onClick )
                {
                    onClick();
                }
                m_isPressed = false;
            }
        }
    }
    else
    {
        m_isHovered = false;
        m_isPressed = false;
    }
}

void GUIButton::update( sf::Time /*deltaTime*/ )
{
    auto targetColor = normalColor;

    if ( this->selectedState )
    {
        targetColor = selectedColor;
    }

    if ( m_isPressed )
    {
        targetColor = activeColor;
    }
    else if ( m_isHovered )
    {
        targetColor = hoverColor;
    }

    m_shape.setFillColor( targetColor );
}
void GUIButton::draw( Window& window ) const
{
    window.draw( m_shape );
    window.draw( text );
}

void GUIButton::adjustTextSize()
{
    auto buttonBounds = m_shape.getGlobalBounds();
    float maxTextWidth = buttonBounds.size.x - 20.f;

    text.setCharacterSize( maxFontSize );

    if ( maxTextWidth > 0 )
    {
        while ( text.getGlobalBounds().size.x > maxTextWidth && text.getCharacterSize() > 5 )
        {
            text.setCharacterSize( text.getCharacterSize() - 1 );
        }
    }

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin( textRect.getCenter() );
    text.setPosition( m_shape.getPosition() );
}