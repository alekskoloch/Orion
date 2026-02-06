#include "GUIEnergyBar.h"
#include "pch.h"

#include "ConfigManager.hpp"
#include "EventManager.h"

#include "energy.h"
#include "player.h"

#include "Window.hpp"

namespace
{
constexpr auto ENERGY_BAR_WIDTH = 300;
constexpr auto ENERGY_BAR_HEIGHT = 50;
constexpr auto ENERGY_BAR_OFFSET = 100;
constexpr auto ENERGY_BAR_OUTLINE_THICKNESS = 5.F;
constexpr auto BLINK_TIMER = 0.2F;
constexpr auto NOT_ENOUGH_ENERGY_TIMER = 1.0F;
} // namespace

GUIEnergyBar::GUIEnergyBar( entt::registry& registry )
    : registry{ registry }, m_blinkTimer{ BLINK_TIMER }, m_notEnoughEnergyTimer{ NOT_ENOUGH_ENERGY_TIMER }
{
    this->initializeEnergyBar();
}

void GUIEnergyBar::update( sf::Time deltaTime )
{
    constexpr auto BLINK_INTERVAL = 0.2F;
    constexpr auto NOT_ENOUGH_ENERGY_DURATION = 2.F;

    EventManager::getInstance().subscribe( EventManager::Event::NotEnoughEnergy, [ this ]() { m_isNotEnoughEnergy = true; } );

    if ( m_isNotEnoughEnergy )
    {
        if ( m_blinkTimer <= 0.F )
        {
            m_blink = !m_blink;
            m_blinkTimer = BLINK_INTERVAL;
        }

        if ( m_blink )
        {
            m_energyBarBackgroundSprite.setOutlineColor( sf::Color::Red );
        }
        else
        {
            m_energyBarBackgroundSprite.setOutlineColor( sf::Color::White );
        }

        m_notEnoughEnergyTimer -= deltaTime.asSeconds();
        m_blinkTimer -= deltaTime.asSeconds();

        if ( m_notEnoughEnergyTimer <= 0.F )
        {
            m_isNotEnoughEnergy = false;
            m_notEnoughEnergyTimer = NOT_ENOUGH_ENERGY_DURATION;
        }
    }
    else
    {
        m_energyBarBackgroundSprite.setOutlineColor( sf::Color::White );
    }

    auto playerEnergyView = registry.view< Player, Energy >();
    for ( auto player : playerEnergyView )
    {
        auto& playerEnergy = playerEnergyView.get< Energy >( player );

        m_energyBarSprite.setSize( sf::Vector2f( ( playerEnergy.currentEnergyValue / playerEnergy.maxEnergyValue ) *
                                                         ENERGY_BAR_WIDTH * ConfigManager::getInstance().getScale(),
                                                     ENERGY_BAR_HEIGHT * ConfigManager::getInstance().getScale() ) );
    }
}

void GUIEnergyBar::draw( Window& window )
{
    window.draw( m_energyBarSprite );
    window.draw( m_energyBarBackgroundSprite );
}

void GUIEnergyBar::initializeEnergyBar()
{
    const auto windowWidth = ConfigManager::getInstance().getScreenWidth();
    const auto windowHeight = ConfigManager::getInstance().getScreenHeight();
    const auto scale = ConfigManager::getInstance().getScale();

    auto position = sf::Vector2f{ static_cast< float >( windowWidth ) - ( ( ENERGY_BAR_WIDTH + ENERGY_BAR_OFFSET ) * scale ),
                                  static_cast< float >( windowHeight ) - ( ( ENERGY_BAR_HEIGHT + ENERGY_BAR_HEIGHT ) * scale ) };

    m_energyBarSprite.setSize( sf::Vector2f( ENERGY_BAR_WIDTH * scale, ENERGY_BAR_HEIGHT * scale ) );
    m_energyBarSprite.setFillColor( sf::Color::Blue );
    m_energyBarSprite.setPosition( position );

    m_energyBarBackgroundSprite.setSize( sf::Vector2f( ENERGY_BAR_WIDTH * scale, ENERGY_BAR_HEIGHT * scale ) );
    m_energyBarBackgroundSprite.setFillColor( sf::Color::Transparent );
    m_energyBarBackgroundSprite.setOutlineColor( sf::Color::White );
    m_energyBarBackgroundSprite.setOutlineThickness(
        std::min( -1.F, std::round( -static_cast< float >( ENERGY_BAR_OUTLINE_THICKNESS ) * scale ) ) );
    m_energyBarBackgroundSprite.setPosition( position );
}