#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <functional>
#include <optional>
#include <variant>

#include "ConfigManager.hpp"
#include "InputContext.hpp"
#include "Window.hpp"
#include "Button.hpp"
#include "Slider.hpp"

class SettingSection
{
public:
    explicit SettingSection( const std::string& label, sf::Vector2f position )
        : m_font( std::make_shared< sf::Font >() )
        , m_label( *m_font )
        , m_position( position )
        , m_basePosition( position )
        , m_labelText( label )
    {
        float scale = ConfigManager::getInstance().getScale();
        m_scale = scale;

        std::string fontPath = std::string( ASSETS_PATH ) + "fonts/ScienceGothic-Regular.ttf";
        if ( !m_font->openFromFile( fontPath ) )
        {
            std::cerr << "ERROR: Font load failed: " << fontPath << std::endl;
        }

        m_label.setCharacterSize( static_cast< unsigned int >( 42 * scale ) );
        m_label.setFillColor( sf::Color( 200, 200, 220 ) );
        m_label.setString( label );

        auto bounds = m_label.getLocalBounds();
        float originX = bounds.position.x + bounds.size.x;
        float originY = bounds.position.y + bounds.size.y / 2.0f;
        m_label.setOrigin( { originX, originY } );

        float labelX = position.x - ( 650.0f * scale / 2.0f ) + ( 40.0f * scale );
        m_label.setPosition( { labelX, position.y } );
    }

    void setButton( const std::string& label, const std::function<void()>& callback )
    {
        float scale = ConfigManager::getInstance().getScale();
        float buttonX = m_position.x + ( 650.0f * scale / 2.0f ) - ( 40.0f * scale );
        
        m_button.emplace( label, sf::Vector2f{ buttonX, m_position.y }, callback, Alignment::Right );
    }

    void setSlider( const std::function<void( float )>& callback,
                    float minValue, float maxValue, float initialValue, ShowValue showValue = ShowValue::Yes )
    {
        float scale = ConfigManager::getInstance().getScale();
        float sliderX = m_position.x + ( 600.0f * scale / 2.0f );
        
        m_slider.emplace( sf::Vector2f{ sliderX, m_position.y }, callback, minValue, maxValue, initialValue, showValue );
    }

    void setScrollOffset( float offset )
    {
        m_scrollOffset = offset;

        float labelX = m_basePosition.x - ( 650.0f * m_scale / 2.0f ) + ( 40.0f * m_scale );
        m_label.setPosition( { labelX, m_basePosition.y - m_scrollOffset } );

        if ( m_button.has_value() )
        {
            auto buttonPos = m_button->getPosition();
            m_button->setPosition( { buttonPos.x, m_basePosition.y - m_scrollOffset } );
        }
        else if ( m_slider.has_value() )
        {
            auto sliderPos = m_slider->getPosition();
            m_slider->setPosition( { sliderPos.x, m_basePosition.y - m_scrollOffset } );
        }
    }

    void setBasePosition( sf::Vector2f pos )
    {
        m_basePosition = pos;
        m_position = pos;
        
        // Update all child elements to new base position
        float labelX = m_basePosition.x - ( 650.0f * m_scale / 2.0f ) + ( 40.0f * m_scale );
        m_label.setPosition( { labelX, m_basePosition.y } );
        
        if ( m_button.has_value() )
        {
            float buttonX = m_basePosition.x + ( 650.0f * m_scale / 2.0f ) - ( 40.0f * m_scale );
            m_button->setPosition( { buttonX, m_basePosition.y } );
        }
        else if ( m_slider.has_value() )
        {
            float sliderX = m_basePosition.x + ( 600.0f * m_scale / 2.0f );
            m_slider->setPosition( { sliderX, m_basePosition.y } );
        }
    }

    sf::Vector2f getBasePosition() const { return m_basePosition; }

    float getScrollOffset() const { return m_scrollOffset; }

    float getHeight() const
    {
        if ( m_button.has_value() )
        {
            return m_button->getSize().y;
        }
        else if ( m_slider.has_value() )
        {
            return m_slider->getSize().y;
        }
        return 100.0f * m_scale; // Default height for label-only sections
    }

    void handleEvent( const InputContext& inputContext )
    {
        if ( m_button.has_value() )
        {
            m_button->handleEvent( inputContext );
        }
        else if ( m_slider.has_value() )
        {
            m_slider->handleEvent( inputContext );
        }
    }

    void update( sf::Time deltaTime )
    {
        if ( m_button.has_value() )
        {
            m_button->update( deltaTime );
        }
        else if ( m_slider.has_value() )
        {
            m_slider->update( deltaTime );
        }
    }

    void draw( Window& window )
    {
        window.draw( m_label );

        if ( m_button.has_value() )
        {
            m_button->draw( window );
        }
        else if ( m_slider.has_value() )
        {
            m_slider->draw( window );
        }
    }

private:
    std::shared_ptr< sf::Font > m_font;
    sf::Text m_label;
    sf::Vector2f m_position;
    sf::Vector2f m_basePosition;
    std::string m_labelText;
    float m_scale{ 1.0f };
    float m_scrollOffset{ 0.0f };

    std::optional< Button > m_button;
    std::optional< Slider > m_slider;
};
