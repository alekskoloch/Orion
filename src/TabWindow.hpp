#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>

#include "InputContext.hpp"
#include "GUIBaseElement.h"
#include "Button.hpp"
#include "Title.hpp"

class Window;

using TabChildElement = GUIBaseElement< Button, Title >;

enum class SettingsTab : std::uint8_t { General, Graphics, Audio, Controls, Count };

class TabWindow
{
public:
    TabWindow( const sf::Vector2f& size, const sf::Vector2f& position );

    TabWindow& addTab( SettingsTab id, const std::string& title, std::vector< TabChildElement >&& content );

    void handleEvent( const InputContext& inputContext );
    void update( sf::Time deltaTime );
    void draw( Window& window );

private:
    struct TabData
    {
        SettingsTab id;
        std::string title;
        sf::RectangleShape shape;
        sf::Text text;
        sf::FloatRect boundingBox;
        float skewOffset;
        bool isHovered{ false };
        
        std::vector< TabChildElement > content;
    };

    void updateTabGeometry();
    bool isPointInsideTab( const sf::Vector2f& point, const TabData& tab ) const;

    SettingsTab m_activeTab{ SettingsTab::General };

    sf::Vector2f m_size;
    sf::Vector2f m_position;
    float m_scale{ 1.0f };

    sf::RectangleShape m_bodyCanvas;
    std::shared_ptr< sf::Shader > m_shader;
    std::shared_ptr< sf::Font > m_font;

    std::vector< TabData > m_tabs;

    sf::Vector2f m_clickPos{ -1000.0f, -1000.0f };
    float m_timeSinceClick{ 100.0f };
};