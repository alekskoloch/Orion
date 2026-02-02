#include "GUIWeaponTile.h"
#include "pch.h"


#include "ConfigManager.hpp"
#include "FontManager.h"

#include "CooldownSystem.h"
#include "SkillSystem.h"

#include "moneyInventory.h"
#include "player.h"
#include "shield.h"
#include "weapon.h"

#include "Window.hpp"

#include "GraphicsOperations.h"

GUIWeaponTile::GUIWeaponTile( entt::registry& registry ) : registry( registry ), weaponIcon{ CreateSprite( "red_weapon" ) }
{
    this->initialize();
}

void GUIWeaponTile::initialize()
{
    this->circle.setRadius( 200.f * ConfigManager::getInstance().getScale() );
    this->circle.setFillColor( sf::Color( 0, 0, 0 ) );
    this->circle.setOutlineColor( sf::Color::White );
    this->circle.setOutlineThickness( 4.f );
    this->circle.setPointCount( 50 );
    this->circle.setOrigin( this->circle.getGlobalBounds().getCenter() );

    this->loadingCircle.setRadius( 100.f * ConfigManager::getInstance().getScale() );
    this->loadingCircle.setFillColor( sf::Color( 255, 0, 0, 50 ) );
    this->circle.setPointCount( 50 );
    this->loadingCircle.setOrigin( this->loadingCircle.getGlobalBounds().getCenter() );
    this->loadingCircle.setPosition( sf::Vector2f{ 0, 0 } );

    this->setWeaponTexture( "red_weapon" );
}

void GUIWeaponTile::setWeaponTexture( const std::string& /*textureName*/ )
{
    this->weaponIcon.setScale( sf::Vector2f{ ConfigManager::getInstance().getScale(), ConfigManager::getInstance().getScale() } );
    float iconWidth = this->weaponIcon.getGlobalBounds().size.x;
    this->weaponIcon.setPosition( sf::Vector2f{ 2120 * ConfigManager::getInstance().getScale() - iconWidth / 2,
                                                120 * ConfigManager::getInstance().getScale() - iconWidth / 2 } );
}

void GUIWeaponTile::update()
{
    // TODO: refactor this
    auto playerWeaponComponent = this->registry.get< Weapon >( this->registry.view< Player >()[ 0 ] );
    if ( playerWeaponComponent.type == WeaponType::SingleShot )
    {
        this->specialShotAvailable = SkillSystem::isSkillEnabled( this->registry, SkillType::SingleShotWeaponSpecialShot );
    }
    else if ( playerWeaponComponent.type == WeaponType::TrippleShot )
    {
        this->specialShotAvailable = SkillSystem::isSkillEnabled( this->registry, SkillType::TripleShotWeaponSpecialShot );
    }

    // TODO: make update loading circle function
    if ( this->specialShotAvailable )
    {
        auto playerEntity = this->registry.view< Player >()[ 0 ];

        if ( CooldownSystem::getCooldown( this->registry, playerEntity, "specialShot" ) != -1.f )
        {
            float cooldown = CooldownSystem::getCooldown( this->registry, playerEntity, "specialShot" );
            // TODO: get this from weapon
            float maxCooldown = 5.f;
            this->loadingCircle.setRadius( ( 200.f * ConfigManager::getInstance().getScale() ) * ( 1 - cooldown / maxCooldown ) );

            this->loadingCircle.setOrigin( { this->loadingCircle.getGlobalBounds().getCenter() } );
            this->loadingCircle.setPosition( sf::Vector2f{ 0, 0 } );
        }
    }
}

void GUIWeaponTile::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
    // TODO: remove this version
    target.draw( this->circle, states );
    if ( this->specialShotAvailable )
    {
        target.draw( this->loadingCircle, states );
    }
    target.draw( this->weaponIcon, states );
}

void GUIWeaponTile::draw( Window& window )
{
    window.draw( this->circle );
    if ( this->specialShotAvailable )
    {
        window.draw( this->loadingCircle );
    }
    window.draw( this->weaponIcon );
}

void GUIWeaponTile::clear()
{
    this->initialize();
    this->specialShotAvailable = false;
}

GUIShieldTile::GUIShieldTile( sf::RenderWindow& window, entt::registry& registry )
    : window( window ), registry( registry ), shieldIcon{ CreateSprite( "basic_shield_ico" ) }
{
    this->initialize();
}

void GUIShieldTile::initialize()
{
    this->circle.setRadius( 150.f * ConfigManager::getInstance().getScale() );
    this->circle.setFillColor( sf::Color( 0, 0, 0 ) );
    this->circle.setOutlineColor( sf::Color::White );
    this->circle.setOutlineThickness( 4.f );
    this->circle.setPointCount( 50 );
    this->circle.setOrigin( this->circle.getGlobalBounds().getCenter() );
    this->circle.setPosition( sf::Vector2f{ 0, 200 * ConfigManager::getInstance().getScale() } );

    this->shiedlStateCircle.setRadius( 0.f );
    this->shiedlStateCircle.setFillColor( sf::Color( 0, 0, 255, 100 ) );
    this->circle.setPointCount( 50 );
    this->shiedlStateCircle.setOrigin( this->shiedlStateCircle.getGlobalBounds().getCenter() );
    this->shiedlStateCircle.setPosition( sf::Vector2f{ 0, 200 * ConfigManager::getInstance().getScale() } );

    this->setShieldTexture( "basic_shield_ico" );
}

void GUIShieldTile::setShieldTexture( const std::string& /*textureName*/ )
{
    this->shieldIcon.setScale(
        sf::Vector2f{ 0.8f * ConfigManager::getInstance().getScale(), 0.8f * ConfigManager::getInstance().getScale() } );
    this->shieldIcon.setPosition(
        sf::Vector2f{ this->circle.getPosition().x + ( 55 * ConfigManager::getInstance().getScale() ),
                      this->circle.getPosition().y + ( 55 * ConfigManager::getInstance().getScale() ) } );
}

void GUIShieldTile::update()
{
    // TODO: make update shield state circle function
    auto playerEntity = this->registry.view< Player >()[ 0 ];

    // TODO: find better way to do this
    if ( !this->registry.get< Shield >( playerEntity ).active &&
         this->shieldDuration != this->registry.get< Shield >( playerEntity ).duration )
    {
        this->shieldDuration = this->registry.get< Shield >( playerEntity ).duration;
    }

    if ( CooldownSystem::getCooldown( this->registry, playerEntity, "shieldCooldown" ) != -1.f )
    {
        float cooldown = CooldownSystem::getCooldown( this->registry, playerEntity, "shieldCooldown" );

        this->shiedlStateCircle.setRadius( ( 150.f * ConfigManager::getInstance().getScale() ) * ( cooldown / shieldDuration ) );

        this->shiedlStateCircle.setOrigin( { this->shiedlStateCircle.getGlobalBounds().getCenter() } );
    }
}

void GUIShieldTile::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( this->circle, states );
    target.draw( this->shiedlStateCircle, states );
    target.draw( this->shieldIcon, states );
}

void GUIShieldTile::clear()
{
    this->initialize();
    this->shieldDuration = 0.F;
}

GUIMoneyBar::GUIMoneyBar( sf::RenderWindow& window, entt::registry& registry )
    : window( window ), registry( registry ), moneyText{ FontManager::getInstance().getFont( "font" ) }
{
    this->bar.setSize(
        sf::Vector2f( 300 * ConfigManager::getInstance().getScale(), 50 * ConfigManager::getInstance().getScale() ) );
    this->bar.setFillColor( sf::Color( 0, 0, 0 ) );
    this->bar.setOutlineColor( sf::Color::White );
    this->bar.setOutlineThickness( 4.f );
    this->bar.setPosition(
        sf::Vector2f{ 100 * ConfigManager::getInstance().getScale(), 0 * ConfigManager::getInstance().getScale() } );

    this->moneyText.setCharacterSize( static_cast< unsigned int >( 30 * ConfigManager::getInstance().getScale() ) );
    this->moneyText.setFillColor( sf::Color::White );
    // TODO: change position
    this->moneyText.setPosition(
        sf::Vector2f{ 250 * ConfigManager::getInstance().getScale(), 15 * ConfigManager::getInstance().getScale() } );
    this->moneyText.setString( "0$" );
}

void GUIMoneyBar::setString( const std::string& string ) { this->moneyText.setString( string ); }

void GUIMoneyBar::update()
{
    auto& playerMoney = this->registry.get< MoneyInventory >( this->registry.view< Player >()[ 0 ] );
    this->moneyText.setString( std::to_string( playerMoney.money ) + "$" );
}

void GUIMoneyBar::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( this->bar, states );
    target.draw( this->moneyText, states );
}