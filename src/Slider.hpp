#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include "ConfigManager.hpp"
#include "InputContext.hpp"
#include "Window.hpp"

class Slider
{
public:
    using Callback = std::function< void( float ) >;

    static constexpr float DEFAULT_WIDTH = 600.f;
    static constexpr float DEFAULT_HEIGHT = 120.f;

    Slider( sf::Vector2f position, Callback callback, float minValue = 0.0f, float maxValue = 100.0f, float initialValue = 50.0f )
        : m_callback( std::move( callback ) ), m_font( std::make_shared< sf::Font >() ), m_valueText( *m_font ),
          m_minValue( minValue ), m_maxValue( maxValue ), m_value( initialValue ), m_currentValue( initialValue ),
          m_releaseTime( -100.0f )
    {
        float scale = ConfigManager::getInstance().getScale();

        m_canvasSize = { DEFAULT_WIDTH * scale, DEFAULT_HEIGHT * scale };
        m_canvas.setSize( m_canvasSize );
        m_canvas.setOrigin( m_canvasSize / 2.f );
        m_canvas.setPosition( position );
        m_canvas.setFillColor( sf::Color::White );

        std::string shaderPath = std::string( ASSETS_PATH ) + "shaders/slider.frag";
        if ( !m_shader.loadFromFile( shaderPath, sf::Shader::Type::Fragment ) )
        {
            std::cerr << "CRITICAL: Slider shader load failed: " << shaderPath << std::endl;
        }

        std::string fontPath = std::string( ASSETS_PATH ) + "fonts/ScienceGothic-Regular.ttf";
        if ( !m_font->openFromFile( fontPath ) )
        {
            std::cerr << "ERROR: Font load failed: " << fontPath << std::endl;
        }

        m_valueText.setCharacterSize( static_cast< unsigned int >( 38 * scale ) );
        m_valueText.setFillColor( sf::Color( 180, 180, 200 ) );

        auto valueBounds = m_valueText.getLocalBounds();
        float valueOriginX = valueBounds.position.x + valueBounds.size.x;
        float valueOriginY = valueBounds.position.y + valueBounds.size.y / 2.0f;
        float valuePosX = m_canvas.getPosition().x + ( DEFAULT_WIDTH * scale / 2.f ) - ( 50.f * scale );
        float valuePosY = m_canvas.getPosition().y - ( 25.f * scale );

        m_valueText.setOrigin( { valueOriginX, valueOriginY } );
        m_valueText.setPosition( { valuePosX, valuePosY } );

        updateValue( initialValue );
    }

    void handleEvent( const InputContext& inputContext )
    {
        const auto& event = inputContext.getEvent();
        const auto& mouseState = inputContext.getMouseState();
        sf::Vector2f mousePos{ mouseState.worldPosition.x, mouseState.worldPosition.y };

        bool isHovered = isPointInside( mousePos );
        m_isHovered = isHovered;

        if ( m_isDragging )
        {
            float scale = ConfigManager::getInstance().getScale();
            float trackHalfWidth = ( DEFAULT_WIDTH * scale * 0.7f ) * 0.5f;
            float trackLeftX = m_canvas.getPosition().x - trackHalfWidth;
            float trackRightX = m_canvas.getPosition().x + trackHalfWidth;

            float clampedX = std::clamp( mousePos.x, trackLeftX, trackRightX );
            float t = ( clampedX - trackLeftX ) / ( trackRightX - trackLeftX );

            updateValue( m_minValue + t * ( m_maxValue - m_minValue ) );

            if ( const auto* released = event.getIf< sf::Event::MouseButtonReleased >() )
            {
                if ( released->button == sf::Mouse::Button::Left )
                {
                    m_isDragging = false;
                    m_releaseTime = m_clock.getElapsedTime().asSeconds();
                }
            }
        }
        else if ( isHovered )
        {
            if ( const auto* pressed = event.getIf< sf::Event::MouseButtonPressed >() )
            {
                if ( pressed->button == sf::Mouse::Button::Left )
                {
                    m_isDragging = true;
                    float scale = ConfigManager::getInstance().getScale();
                    float trackHalfWidth = ( DEFAULT_WIDTH * scale * 0.7f ) * 0.5f;
                    float trackLeftX = m_canvas.getPosition().x - trackHalfWidth;
                    float trackRightX = m_canvas.getPosition().x + trackHalfWidth;

                    float clampedX = std::clamp( mousePos.x, trackLeftX, trackRightX );
                    float t = ( clampedX - trackLeftX ) / ( trackRightX - trackLeftX );

                    updateValue( m_minValue + t * ( m_maxValue - m_minValue ) );
                }
            }
        }

        m_mouseLocalPos = mousePos - m_canvas.getPosition();
    }

    void update( sf::Time deltaTime )
    {
        m_shader.setUniform( "u_time", m_clock.getElapsedTime().asSeconds() );
        m_shader.setUniform( "u_hover", m_isHovered ? 1.0f : 0.0f );
        m_shader.setUniform( "u_dragging", m_isDragging ? 1.0f : 0.0f );
        m_shader.setUniform( "u_releaseTime", m_releaseTime );

        std::string valueStr = formatValue( m_currentValue );
        m_valueText.setString( valueStr );
    }

    void setValue( float value ) { updateValue( value ); }

    float getValue() const { return m_currentValue; }

    void draw( Window& window )
    {
        float scale = ConfigManager::getInstance().getScale();

        m_shader.setUniform( "u_pos", m_canvas.getPosition() );
        m_shader.setUniform( "u_winHeight", static_cast< float >( ConfigManager::getInstance().getScreenHeight() ) );
        m_shader.setUniform( "u_size", sf::Vector2f( m_canvasSize.x, m_canvasSize.y ) );
        m_shader.setUniform( "u_value", m_normalizedValue );
        m_shader.setUniform( "u_mouse", m_mouseLocalPos );

        window.draw( m_canvas, &m_shader );
        window.draw( m_valueText );
    }

private:
    bool isPointInside( sf::Vector2f point )
    {
        sf::Vector2f p = point - m_canvas.getPosition();
        float halfW = m_canvasSize.x / 2.f;
        float halfH = m_canvasSize.y / 2.f;

        return std::abs( p.x ) <= halfW && std::abs( p.y ) <= halfH;
    }

    void updateValue( float value )
    {
        m_currentValue = std::clamp( value, m_minValue, m_maxValue );
        m_normalizedValue = ( m_currentValue - m_minValue ) / ( m_maxValue - m_minValue );

        if ( m_callback )
        {
            m_callback( m_currentValue );
        }
    }

    std::string formatValue( float value ) const
    {
        float rounded = std::round( value * 10.0f ) / 10.0f;
        int intPart = static_cast< int >( rounded );
        float fracPart = rounded - intPart;

        if ( std::abs( fracPart ) < 0.05F )
        {
            return std::to_string( intPart );
        }
        
                    std::string str = std::to_string( static_cast< int >( rounded * 10 ) );
            if ( str.size() >= 1 )
            {
                str.insert( str.size() - 1, "." );
            }
            return str;
       
    }

    Callback m_callback;
    sf::RectangleShape m_canvas;
    sf::Vector2f m_canvasSize;
    sf::Shader m_shader;
    sf::Clock m_clock;
    std::shared_ptr< sf::Font > m_font;
    sf::Text m_valueText;
    float m_minValue;
    float m_maxValue;
    float m_value;
    float m_currentValue;
    float m_normalizedValue;
    bool m_isHovered = false;
    bool m_isDragging = false;
    sf::Vector2f m_mouseLocalPos;
    float m_releaseTime;
};
