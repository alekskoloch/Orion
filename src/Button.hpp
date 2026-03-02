#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>


#include "ConfigManager.hpp"
#include "InputContext.hpp"
#include "Window.hpp"

enum class Alignment : std::uint8_t
{
    Center,
    Right,
    Left
};

class Button
{
public:
    using Callback = std::function< void() >;

    static constexpr float DEFAULT_WIDTH = 620.f;
    static constexpr float DEFAULT_HEIGHT = 130.f;

    static constexpr float EXPANSION_AMOUNT = 30.f;
    static constexpr float ANIM_SPEED = 15.0f;

    static constexpr float CANVAS_MARGIN = 30.f;

    Button( std::string_view text, sf::Vector2f position, Callback callback, Alignment align = Alignment::Right )
        : m_callback( std::move( callback ) ), m_font( std::make_shared< sf::Font >() ), m_text( *m_font ), m_alignment( align ),
          m_skew( 0.5f ), m_currentWidth( DEFAULT_WIDTH )
    {
        float scale = ConfigManager::getInstance().getScale();

        float maxPossibleWidth = DEFAULT_WIDTH + EXPANSION_AMOUNT + CANVAS_MARGIN;

        m_canvasSize = { maxPossibleWidth * scale, ( DEFAULT_HEIGHT + CANVAS_MARGIN ) * scale };
        m_canvas.setSize( m_canvasSize );
        m_canvas.setOrigin( m_canvasSize / 2.f );
        m_canvas.setPosition( position );
        m_canvas.setFillColor( sf::Color::White );

        std::string shaderPath = std::string( ASSETS_PATH ) + "shaders/button.frag";
        if ( !m_shader.loadFromFile( shaderPath, sf::Shader::Type::Fragment ) )
        {
            std::cerr << "CRITICAL: Shader load failed: " << shaderPath << std::endl;
        }

        std::string fontPath = std::string( ASSETS_PATH ) + "fonts/ScienceGothic-Regular.ttf";
        if ( !m_font->openFromFile( fontPath ) )
        {
            std::cerr << "ERROR: Font load failed: " << fontPath << std::endl;
        }

        m_text.setString( std::string( text ) );
        m_text.setCharacterSize( static_cast< unsigned int >( 40 * scale ) );
        m_text.setFillColor( sf::Color::White );

        updateTextGeometry();
    }

    void handleEvent( const InputContext& inputContext )
    {
        const auto& event = inputContext.getEvent();
        const auto& mouseState = inputContext.getMouseState();
        sf::Vector2f mousePos{ mouseState.worldPosition.x, mouseState.worldPosition.y };

        bool isInside = isPointInside( mousePos );
        m_isHovered = isInside;

        float scale = ConfigManager::getInstance().getScale();
        float widthDelta = ( m_currentWidth - DEFAULT_WIDTH ) * scale;
        float centerShift = widthDelta / 2.0f;

        sf::Vector2f effectiveCenter = m_canvas.getPosition();
        effectiveCenter.x -= centerShift;

        m_mouseLocalPos = mousePos - effectiveCenter;

        if ( m_isHovered )
            m_text.setFillColor( sf::Color( 220, 220, 255 ) );
        else
            m_text.setFillColor( sf::Color( 200, 200, 200 ) );

        if ( const auto* pressed = event.getIf< sf::Event::MouseButtonPressed >() )
        {
            if ( pressed->button == sf::Mouse::Button::Left && isInside && m_callback )
            {
                m_callback();
            }
        }
    }

    void update( sf::Time deltaTime )
    {
        float targetWidth = m_isHovered ? ( DEFAULT_WIDTH + EXPANSION_AMOUNT ) : DEFAULT_WIDTH;

        float diff = targetWidth - m_currentWidth;
        if ( std::abs( diff ) > 0.1f )
        {
            m_currentWidth += diff * ANIM_SPEED * deltaTime.asSeconds();
        }
        else
        {
            m_currentWidth = targetWidth;
        }

        m_shader.setUniform( "u_time", m_clock.getElapsedTime().asSeconds() );
    }

    void setText( std::string_view t )
    {
        m_text.setString( std::string( t ) );
        updateTextGeometry();
    }
    void setAlignment( Alignment a )
    {
        m_alignment = a;
        updateTextGeometry();
    }
    void setSkew( float s ) { m_skew = s; }

    void draw( Window& window )
    {
        float scale = ConfigManager::getInstance().getScale();

        float widthDelta = ( m_currentWidth - DEFAULT_WIDTH ) * scale;
        float centerShiftX = widthDelta / 2.0f;

        sf::Vector2f shaderPos = m_canvas.getPosition();
        shaderPos.x -= centerShiftX;

        m_shader.setUniform( "u_pos", shaderPos );
        m_shader.setUniform( "u_winHeight", static_cast< float >( ConfigManager::getInstance().getScreenHeight() ) );

        m_shader.setUniform( "u_size", sf::Vector2f( m_currentWidth * scale, DEFAULT_HEIGHT * scale ) );

        m_shader.setUniform( "u_time", m_clock.getElapsedTime().asSeconds() );
        m_shader.setUniform( "u_skew", m_skew );
        m_shader.setUniform( "u_mouse", m_mouseLocalPos );

        window.draw( m_canvas, &m_shader );
        window.draw( m_text );
    }

private:
    bool isPointInside( sf::Vector2f point )
    {
        float scale = ConfigManager::getInstance().getScale();

        float widthDelta = ( m_currentWidth - DEFAULT_WIDTH ) * scale;
        float centerShiftX = widthDelta / 2.0f;
        sf::Vector2f effectiveCenter = m_canvas.getPosition();
        effectiveCenter.x -= centerShiftX;

        sf::Vector2f p = point - effectiveCenter;

        float halfW = ( m_currentWidth * scale ) * 0.5f;
        float halfH = ( DEFAULT_HEIGHT * scale ) * 0.5f;

        float d_y = std::abs( p.y ) - halfH;
        float d_right = p.x - halfW;

        sf::Vector2f normalLeft{ -1.0f, m_skew };
        float len = std::sqrt( normalLeft.x * normalLeft.x + normalLeft.y * normalLeft.y );
        normalLeft /= len;

        float leftAnchor = -halfW + ( m_skew * halfH * 0.5f );
        float d_left = ( p.x - leftAnchor ) * normalLeft.x + p.y * normalLeft.y;

        float d = std::max( { d_y, d_right, d_left } );
        return d <= 0.0f;
    }

    void updateTextGeometry()
    {
        auto textBounds = m_text.getLocalBounds();
        float scale = ConfigManager::getInstance().getScale();
        float btnWidth = DEFAULT_WIDTH * scale;
        sf::Text refText( *m_font, "A", m_text.getCharacterSize() );
        auto refBounds = refText.getLocalBounds();

        float paddingX = 40.0f * scale;

        float originY = refBounds.position.y + refBounds.size.y / 2.0f;
        float posY = m_canvas.getPosition().y;
        float originX = 0.0f;
        float posX = 0.0f;

        switch ( m_alignment )
        {
        case Alignment::Center:
            originX = textBounds.position.x + textBounds.size.x / 2.0f;
            posX = m_canvas.getPosition().x;
            break;
        case Alignment::Right:
            originX = textBounds.position.x + textBounds.size.x;

            posX = m_canvas.getPosition().x + ( btnWidth / 2.0f ) - paddingX;
            break;
        case Alignment::Left:
            originX = textBounds.position.x;
            posX = m_canvas.getPosition().x - ( btnWidth / 2.0f ) + paddingX + ( m_skew * 40.f );
            break;
        }
        m_text.setOrigin( { originX, originY } );
        m_text.setPosition( { posX, posY } );
    }

    Callback m_callback;
    sf::RectangleShape m_canvas;
    sf::Vector2f m_canvasSize;
    sf::Shader m_shader;
    sf::Clock m_clock;
    std::shared_ptr< sf::Font > m_font;
    sf::Text m_text;
    Alignment m_alignment;
    bool m_isHovered = false;
    float m_skew;
    sf::Vector2f m_mouseLocalPos;

    float m_currentWidth;
};