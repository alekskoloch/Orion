#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>

#include "InputContext.hpp"
#include "Button.hpp"
#include "Title.hpp"
#include "Slider.hpp"
#include "SettingSection.hpp"
#include "Scrollbar.hpp"

class Window;

// TabChildElement is a variant of raw widget types, not GUIBaseElement
// This allows direct access to setPosition/getPosition/getSize for scrolling
using TabChildElement = std::variant< Button, Title, Slider, SettingSection >;

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
        float contentHeight{ 0.0f };
        float contentStartY{ 0.0f };  // Y position where content starts (for scroll calculation)
    };

    void updateTabGeometry();
    bool isPointInsideTab( const sf::Vector2f& point, const TabData& tab ) const;
    void calculateContentHeight( TabData& tab );
    void updateScrollbar();

    SettingsTab m_activeTab{ SettingsTab::General };

    sf::Vector2f m_size;
    sf::Vector2f m_position;
    float m_scale{ 1.0f };

    sf::RectangleShape m_bodyCanvas;
    std::shared_ptr< sf::Shader > m_shader;
    std::shared_ptr< sf::Font > m_font;

    std::vector< TabData > m_tabs;

    std::unique_ptr< Scrollbar > m_scrollbar;
    float m_contentStartY{ 0.0f };
    float m_contentSpacing{ 0.0f };

    sf::Vector2f m_clickPos{ -1000.0f, -1000.0f };
    float m_timeSinceClick{ 100.0f };
};