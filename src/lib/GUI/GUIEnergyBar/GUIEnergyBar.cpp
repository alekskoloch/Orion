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
} // namespace

GUIEnergyBar::GUIEnergyBar( entt::registry& registry ) : registry( registry )
{
    this->initializeEnergyBar();
}

void GUIEnergyBar::update( sf::Time deltaTime )
{
    constexpr auto BLINK_INTERVAL = 0.2F;
    constexpr auto NOT_ENOUGH_ENERGY_DURATION = 2.F;

    EventManager::getInstance().subscribe( EventManager::Event::NotEnoughEnergy, [ this ]() { this->isNotEnoughEnergy = true; } );

    if ( this->isNotEnoughEnergy )
    {
        if ( this->blinkTimer <= 0.F )
        {
            this->blink = !this->blink;
            this->blinkTimer = BLINK_INTERVAL;
        }

        if ( this->blink )
        {
            this->energyBarBackgroundSprite.setOutlineColor( sf::Color::Red );
        }
        else
        {
            this->energyBarBackgroundSprite.setOutlineColor( sf::Color::White );
        }

        this->notEnoughEnergyTimer -= deltaTime.asSeconds();
        this->blinkTimer -= deltaTime.asSeconds();

        if ( this->notEnoughEnergyTimer <= 0.F )
        {
            this->isNotEnoughEnergy = false;
            this->notEnoughEnergyTimer = NOT_ENOUGH_ENERGY_DURATION;
        }
    }
    else
    {
        this->energyBarBackgroundSprite.setOutlineColor( sf::Color::White );
    }

    auto playerEnergyView = this->registry.view< Player, Energy >();
    for ( auto player : playerEnergyView )
    {
        auto& playerEnergy = playerEnergyView.get< Energy >( player );

        this->energyBarSprite.setSize( sf::Vector2f( ( playerEnergy.currentEnergyValue / playerEnergy.maxEnergyValue ) *
                                                         ENERGY_BAR_WIDTH * ConfigManager::getInstance().getScale(),
                                                     ENERGY_BAR_HEIGHT * ConfigManager::getInstance().getScale() ) );
    }
}

void GUIEnergyBar::draw( Window& window )
{
    window.draw( energyBarSprite );
    window.draw( energyBarBackgroundSprite );
}

void GUIEnergyBar::initializeEnergyBar()
{
    const auto windowWidth = ConfigManager::getInstance().getScreenWidth();
    const auto windowHeight = ConfigManager::getInstance().getScreenHeight();
    const auto scale = ConfigManager::getInstance().getScale();

    auto position = sf::Vector2f{ static_cast< float >( windowWidth ) - ( ( ENERGY_BAR_WIDTH + ENERGY_BAR_OFFSET ) * scale ),
                                  static_cast< float >( windowHeight ) - ( ( ENERGY_BAR_HEIGHT + ENERGY_BAR_HEIGHT ) * scale ) };

    this->energyBarSprite.setSize( sf::Vector2f( ENERGY_BAR_WIDTH * scale, ENERGY_BAR_HEIGHT * scale ) );
    this->energyBarSprite.setFillColor( sf::Color::Blue );
    this->energyBarSprite.setPosition( position );

    this->energyBarBackgroundSprite.setSize( sf::Vector2f( ENERGY_BAR_WIDTH * scale, ENERGY_BAR_HEIGHT * scale ) );
    this->energyBarBackgroundSprite.setFillColor( sf::Color::Transparent );
    this->energyBarBackgroundSprite.setOutlineColor( sf::Color::White );
    this->energyBarBackgroundSprite.setOutlineThickness(
        std::min( -1.F, std::round( -static_cast< float >( ENERGY_BAR_OUTLINE_THICKNESS ) * scale ) ) );
    this->energyBarBackgroundSprite.setPosition( position );
}