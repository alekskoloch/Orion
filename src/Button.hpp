#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
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

    Button( std::string_view textureName, sf::Vector2f position, Callback callback )
        : m_callback( std::move( callback ) ), m_sprite( TextureManager::getInstance().getTexture( std::string( textureName ) ) )
    {
        float scale = ConfigManager::getInstance().getScale();
        m_sprite.setScale( { scale, scale } );

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

        if ( const auto* moved = event.getIf< sf::Event::MouseMoved >() )
        {
            if ( isInside )
            {
                m_isHovered = true;
                m_sprite.setColor( sf::Color( 200, 200, 200 ) );
            }
            else
            {
                m_isHovered = false;
                m_sprite.setColor( sf::Color::White );
            }
        }

        if ( const auto* pressed = event.getIf< sf::Event::MouseButtonPressed >() )
        {
            if ( pressed->button == sf::Mouse::Button::Left )
            {
                if ( isInside )
                {
                    if ( m_callback )
                    {
                        m_callback();
                    }
                }
            }
        }
    }

    void update( sf::Time deltaTime ) {}

    void draw( Window& window ) { window.draw( m_sprite ); }

private:
    Callback m_callback;
    sf::Sprite m_sprite;
    bool m_isHovered = false;
};