#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "InputContext.hpp"

class Window;

enum class SettingsTab : std::uint8_t { General, Graphics, Audio, Controls, Count };

struct TabElement
{
    explicit TabElement(const sf::Font& font) : text(font) {}
    
    std::string title;
    SettingsTab id;
    sf::RectangleShape shape; 
    sf::Text text;
    sf::FloatRect boundingBox; 
    float skewOffset; 
    bool isHovered{ false };
};

class TabWindow
{
public:
    TabWindow();
    void handleEvent( const InputContext& inputContext );
    void update( sf::Time deltaTime );
    void draw( Window& window );
    void setActiveTab( SettingsTab tab );

private:
    void initGeometry();
    bool isPointInsideTab(const sf::Vector2f& point, const TabElement& tab);

    SettingsTab m_activeTab{ SettingsTab::General };
    
    sf::RectangleShape m_bodyCanvas;
    std::shared_ptr< sf::Shader > m_bodyShader; 
    std::shared_ptr< sf::Shader > m_tabShader;

    sf::Vector2f m_bodySize;
    sf::Vector2f m_bodyPosition;

    std::vector< TabElement > m_tabs;
    std::shared_ptr< sf::Font > m_font;
    float m_scale{ 1.0f };

    // --- NOWE ZMIENNE DO EFEKTU FALI ---
    sf::Vector2f m_clickPos{ -1000.0f, -1000.0f }; // Pozycja kliknięcia
    float m_timeSinceClick{ 10.0f }; // Czas od kliknięcia (startowo duży, żeby nie było fali)
};