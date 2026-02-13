#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include "ConfigManager.hpp"
#include "Window.hpp"

//
#ifndef ASSETS_PATH
#define ASSETS_PATH "assets/"
#endif

class Title
{
public:
    Title( std::string_view text, sf::Vector2f position, unsigned int fontSize = 150 )
        : m_font( std::make_shared< sf::Font >() ), m_text( *m_font )
    {
        std::string fontPath = std::string( ASSETS_PATH ) + "fonts/ScienceGothic-Regular.ttf";
        if ( !m_font->openFromFile( fontPath ) )
        {
            std::cerr << "ERROR: Failed to load font for Title: " << fontPath << std::endl;
        }

        float scale = ConfigManager::getInstance().getScale();

        m_text.setString( std::string( text ) );
        m_text.setCharacterSize( static_cast< unsigned int >( fontSize * scale ) );
        m_text.setFillColor( sf::Color::White );

        auto bounds = m_text.getLocalBounds();

        m_text.setOrigin( { bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f } );

        m_text.setPosition( position );
    }

    void draw( Window& window ) { window.draw( m_text ); }

    void update( sf::Time ) {}

private:
    std::shared_ptr< sf::Font > m_font;
    sf::Text m_text;
};