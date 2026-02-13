#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <string_view>

#include "ConfigManager.hpp"
#include "InputContext.hpp"
#include "TextureManager.h"
#include "Window.hpp"

#ifndef ASSETS_PATH
    #define ASSETS_PATH "assets/"
#endif

enum class Alignment
{
    Center,
    Right,
    Left
};

class Button
{
public:
    using Callback = std::function< void() >;

    // TODO: Sprite Wrapper
    static constexpr int FRAME_W = 960;
    static constexpr int FRAME_H = 257;
    static constexpr int GAP = 2;
    static constexpr int START_X = 1;
    static constexpr int START_Y = 1;
    static constexpr int TOTAL_FRAMES = 48;
    static constexpr float FPS = 600.0F;

    Button( std::string_view textureName, std::string_view text, sf::Vector2f position, Callback callback, Alignment align = Alignment::Right )
        : m_callback( std::move( callback ) )
        , m_sprite( TextureManager::getInstance().getTexture( std::string( textureName ) ) )
        , m_font( std::make_shared< sf::Font >() )
        , m_text( *m_font )
        , m_alignment( align )
    {
        float scale = ConfigManager::getInstance().getScale();
        m_sprite.setScale( { scale, scale } );

        updateTextureRect();

        auto bounds = m_sprite.getLocalBounds();
        m_sprite.setOrigin( { bounds.size.x / 2.F, bounds.size.y / 2.F } );
        m_sprite.setPosition( position );

        std::string fontPath = std::string( ASSETS_PATH ) + "fonts/ScienceGothic-Regular.ttf";
        if ( !m_font->openFromFile( fontPath ) )
        {
            std::cerr << "ERROR: Failed to load font: " << fontPath << std::endl;
        }

        m_text.setString( std::string( text ) );
        m_text.setCharacterSize( static_cast< unsigned int >( 70 * scale ) );
        m_text.setFillColor( sf::Color::White );

        updateTextGeometry();
    }

    void setText( std::string_view text )
    {
        m_text.setString( std::string( text ) );
        updateTextGeometry();
    }

    void setAlignment( Alignment align )
    {
        m_alignment = align;
        updateTextGeometry();
    }

    void handleEvent( const InputContext& inputContext )
    {
        const auto& event = inputContext.getEvent();
        const auto& mouseState = inputContext.getMouseState();
        sf::Vector2f mousePos{ mouseState.worldPosition.x, mouseState.worldPosition.y };

        bool isInside = m_sprite.getGlobalBounds().contains( mousePos );
        m_isHovered = isInside;

        if ( m_isHovered )
             m_text.setFillColor( sf::Color( 220, 220, 255 ) );
        else
             m_text.setFillColor( sf::Color::White );

        if ( const auto* pressed = event.getIf< sf::Event::MouseButtonPressed >() )
        {
            if ( pressed->button == sf::Mouse::Button::Left && isInside )
            {
                if ( m_callback ) m_callback();
            }
        }
    }

    void update( sf::Time deltaTime )
    {
        float deltaTimeAsSeconds = deltaTime.asSeconds();
        bool frameChanged = false;

        if ( m_isHovered )
        {
            if ( m_currentFrame < TOTAL_FRAMES - 1 )
            {
                m_currentFrame += FPS * deltaTimeAsSeconds;
                if ( m_currentFrame >= TOTAL_FRAMES ) m_currentFrame = TOTAL_FRAMES - 1;
                frameChanged = true;
            }
        }
        else
        {
            if ( m_currentFrame > 0 )
            {
                m_currentFrame -= FPS * deltaTimeAsSeconds;
                m_currentFrame = std::max< float >( m_currentFrame, 0 );
                frameChanged = true;
            }
        }

        if ( frameChanged ) updateTextureRect();
    }

    void draw( Window& window ) 
    { 
        window.draw( m_sprite ); 
        window.draw( m_text );
    }

private:
    void updateTextureRect()
    {
        int frameIndex = static_cast< int >( std::floor( m_currentFrame ) );
        int col = 0, row = 0;

        if ( frameIndex <= 38 ) { col = frameIndex % 3; row = frameIndex / 3; }
        else { col = 3; row = frameIndex - 39; }

        int posX = START_X + ( col * ( FRAME_W + GAP ) );
        int posY = START_Y + ( row * ( FRAME_H + GAP ) );

        m_sprite.setTextureRect( sf::IntRect( { posX, posY }, { FRAME_W, FRAME_H } ) );
    }

    void updateTextGeometry()
    {
        auto textBounds = m_text.getLocalBounds();
        auto btnBounds = m_sprite.getGlobalBounds();

        sf::Text refText( *m_font, "H", m_text.getCharacterSize() );
        auto refBounds = refText.getLocalBounds();

        float paddingX = 100.0f * m_sprite.getScale().x;

        float originY = refBounds.position.y + refBounds.size.y / 2.0f;
        
        float originX = 0.0f;
        float posY = m_sprite.getPosition().y;
        float posX = 0.0f;

        switch ( m_alignment )
        {
            case Alignment::Center:
                originX = textBounds.position.x + textBounds.size.x / 2.0f;
                posX = m_sprite.getPosition().x;
                break;

            case Alignment::Right:
                originX = textBounds.position.x + textBounds.size.x;
                posX = m_sprite.getPosition().x + (btnBounds.size.x / 2.0f) - paddingX;
                break;

            case Alignment::Left:
                originX = textBounds.position.x;
                posX = m_sprite.getPosition().x - (btnBounds.size.x / 2.0f) + paddingX;
                break;
        }

        m_text.setOrigin( { originX, originY } );
        m_text.setPosition( { posX, posY } );
    }

    Callback m_callback;
    sf::Sprite m_sprite;
    std::shared_ptr< sf::Font > m_font;
    sf::Text m_text;
    
    Alignment m_alignment;
    bool m_isHovered = false;
    float m_currentFrame = 0.0f;
};