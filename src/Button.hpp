#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <cmath>
#include <functional>
#include <string_view>

#include "ConfigManager.hpp"
#include "InputContext.hpp"
#include "TextureManager.h"
#include "Window.hpp"

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

    Button( std::string_view textureName, sf::Vector2f position, Callback callback )
        : m_callback( std::move( callback ) ), m_sprite( TextureManager::getInstance().getTexture( std::string( textureName ) ) )
    {
        float scale = ConfigManager::getInstance().getScale();
        m_sprite.setScale( { scale, scale } );

        updateTextureRect();

        auto bounds = m_sprite.getLocalBounds();

        m_sprite.setOrigin( { bounds.size.x / 2.F, bounds.size.y / 2.F } );
        m_sprite.setPosition( position );
    }

    void handleEvent( const InputContext& inputContext )
    {
        const auto& event = inputContext.getEvent();
        const auto& mouseState = inputContext.getMouseState();
        sf::Vector2f mousePos{ mouseState.worldPosition.x, mouseState.worldPosition.y };

        bool isInside = m_sprite.getGlobalBounds().contains( mousePos );

        m_isHovered = isInside;

        if ( const auto* pressed = event.getIf< sf::Event::MouseButtonPressed >() )
        {
            if ( pressed->button == sf::Mouse::Button::Left && isInside )
            {
                if ( m_callback )
                {
                    m_callback();
                }
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
                if ( m_currentFrame >= TOTAL_FRAMES )
                {
                    m_currentFrame = TOTAL_FRAMES - 1;
                }
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

        if ( frameChanged )
        {
            updateTextureRect();
        }
    }

    void draw( Window& window ) { window.draw( m_sprite ); }

private:
    void updateTextureRect()
    {
        int frameIndex = static_cast< int >( std::floor( m_currentFrame ) );

        int col = 0;
        int row = 0;

        if ( frameIndex <= 38 )
        {
            col = frameIndex % 3;
            row = frameIndex / 3;
        }

        else
        {
            col = 3;
            row = frameIndex - 39;
        }

        int posX = START_X + ( col * ( FRAME_W + GAP ) );
        int posY = START_Y + ( row * ( FRAME_H + GAP ) );

        m_sprite.setTextureRect( sf::IntRect( { posX, posY }, { FRAME_W, FRAME_H } ) );
    }

    Callback m_callback;
    sf::Sprite m_sprite;
    bool m_isHovered = false;
    float m_currentFrame = 0.0f;
};