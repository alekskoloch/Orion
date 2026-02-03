#include "GUIWeaponTile.h"
#include "pch.h"

#include "ConfigManager.hpp"
#include "CooldownSystem.h"
#include "FontManager.h"
#include "GraphicsOperations.h"
#include "SkillSystem.h"
#include "Window.hpp"
#include "moneyInventory.h"
#include "player.h"
#include "shield.h"
#include "weapon.h"


GUIWeaponTile::GUIWeaponTile( entt::registry& registry ) : registry( registry ), weaponIcon{ CreateSprite( "red_weapon" ) }
{
    this->initialize();
}

void GUIWeaponTile::initialize()
{
    float scale = ConfigManager::getInstance().getScale();
    float radius = 200.f * scale;

    this->circle.setRadius( radius );
    this->circle.setFillColor( sf::Color( 0, 0, 0 ) );
    this->circle.setOutlineColor( sf::Color::White );
    this->circle.setOutlineThickness( 4.f );
    this->circle.setPointCount( 100 );
    this->circle.setOrigin( { radius, radius } );
    this->circle.setPosition( { 0.f, 0.f } );

    this->loadingCircle.setFillColor( sf::Color( 255, 0, 0, 50 ) );
    this->loadingCircle.setPointCount( 100 );

    this->setWeaponTexture( "red_weapon" );
}

void GUIWeaponTile::setWeaponTexture( const std::string& textureName )
{
    float scale = ConfigManager::getInstance().getScale();
    this->weaponIcon.setScale( { scale, scale } );

    sf::FloatRect bounds = this->weaponIcon.getLocalBounds();
    this->weaponIcon.setOrigin( { bounds.size.x / 2.f, bounds.size.y / 2.f } );
    this->weaponIcon.setPosition( { 50.f * scale, 50.f * scale } );
}

void GUIWeaponTile::update()
{
    auto view = this->registry.view< Player >();
    if ( view.empty() )
        return;

    auto playerEntity = view[ 0 ];
    auto& playerWeaponComponent = this->registry.get< Weapon >( playerEntity );

    if ( playerWeaponComponent.type == WeaponType::SingleShot )
    {
        this->specialShotAvailable = SkillSystem::isSkillEnabled( this->registry, SkillType::SingleShotWeaponSpecialShot );
    }
    else if ( playerWeaponComponent.type == WeaponType::TrippleShot )
    {
        this->specialShotAvailable = SkillSystem::isSkillEnabled( this->registry, SkillType::TripleShotWeaponSpecialShot );
    }

    if ( this->specialShotAvailable )
    {
        float cooldown = CooldownSystem::getCooldown( this->registry, playerEntity, "specialShot" );
        if ( cooldown != -1.f )
        {
            float scale = ConfigManager::getInstance().getScale();
            float maxCooldown = 5.f;
            float currentRadius = ( 200.f * scale ) * ( 1.f - ( cooldown / maxCooldown ) );

            this->loadingCircle.setRadius( currentRadius );
            this->loadingCircle.setOrigin( { currentRadius, currentRadius } );
            this->loadingCircle.setPosition( { 0.f, 0.f } );
        }
    }
}

void GUIWeaponTile::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
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

GUIShieldTile::GUIShieldTile( entt::registry& registry ) : registry( registry ), shieldIcon{ CreateSprite( "basic_shield_ico" ) }
{
    this->initialize();
}

void GUIShieldTile::initialize()
{
    float scale = ConfigManager::getInstance().getScale();
    float radius = 150.f * scale;

    this->circle.setRadius( radius );
    this->circle.setFillColor( sf::Color( 0, 0, 0 ) );
    this->circle.setOutlineColor( sf::Color::White );
    this->circle.setOutlineThickness( 4.f );
    this->circle.setPointCount( 100 );
    this->circle.setOrigin( { radius, radius } );
    this->circle.setPosition( { 0.f, 250.f * scale } );

    this->shiedlStateCircle.setFillColor( sf::Color( 0, 0, 255, 100 ) );
    this->shiedlStateCircle.setPointCount( 100 );

    this->setShieldTexture( "basic_shield_ico" );
}

void GUIShieldTile::setShieldTexture( const std::string& textureName )
{
    float scale = ConfigManager::getInstance().getScale();
    this->shieldIcon.setScale( { 0.8f * scale, 0.8f * scale } );

    sf::FloatRect bounds = this->shieldIcon.getLocalBounds();
    this->shieldIcon.setOrigin( { bounds.size.x / 2.f, bounds.size.y / 2.f } );
    this->shieldIcon.setPosition( { 40.f * scale, 290.f * scale } );
}

void GUIShieldTile::update()
{
    auto view = this->registry.view< Player >();
    if ( view.empty() )
        return;

    auto playerEntity = view[ 0 ];
    auto& shieldComp = this->registry.get< Shield >( playerEntity );

    if ( !shieldComp.active && this->shieldDuration != shieldComp.duration )
    {
        this->shieldDuration = shieldComp.duration;
    }

    float cooldown = CooldownSystem::getCooldown( this->registry, playerEntity, "shieldCooldown" );
    if ( cooldown != -1.f && shieldDuration > 0.f )
    {
        float scale = ConfigManager::getInstance().getScale();
        float currentRadius = ( 150.f * scale ) * ( cooldown / shieldDuration );

        this->shiedlStateCircle.setRadius( currentRadius );
        this->shiedlStateCircle.setOrigin( { currentRadius, currentRadius } );
        this->shiedlStateCircle.setPosition( this->circle.getPosition() );
    }
    else
    {
        this->shiedlStateCircle.setRadius( 0.f );
    }
}

void GUIShieldTile::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( this->circle, states );
    target.draw( this->shiedlStateCircle, states );
    target.draw( this->shieldIcon, states );
}

void GUIShieldTile::draw( Window& window )
{
    window.draw( this->circle );
    window.draw( this->shiedlStateCircle );
    window.draw( this->shieldIcon );
}

void GUIShieldTile::clear()
{
    this->initialize();
    this->shieldDuration = 0.f;
}

GUIMoneyBar::GUIMoneyBar( entt::registry& registry )
    : registry( registry ), moneyText{ FontManager::getInstance().getFont( "font" ) }
{
    float scale = ConfigManager::getInstance().getScale();

    this->bar.setSize( { 300.f * scale, 50.f * scale } );
    this->bar.setFillColor( sf::Color( 0, 0, 0 ) );
    this->bar.setOutlineColor( sf::Color::White );
    this->bar.setOutlineThickness( 4.f );
    this->bar.setPosition( { 150.f * scale, 10.f * scale } );

    this->moneyText.setCharacterSize( static_cast< unsigned int >( 30 * scale ) );
    this->moneyText.setFillColor( sf::Color::White );
    this->moneyText.setPosition( { 170.f * scale, 15.f * scale } );
    this->moneyText.setString( "0$" );
}

void GUIMoneyBar::setString( const std::string& string ) { this->moneyText.setString( string ); }

void GUIMoneyBar::update()
{
    auto view = this->registry.view< Player >();
    if ( view.empty() )
        return;

    auto& playerMoney = this->registry.get< MoneyInventory >( view[ 0 ] );
    this->moneyText.setString( std::to_string( playerMoney.money ) + "$" );
}

void GUIMoneyBar::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( this->bar, states );
    target.draw( this->moneyText, states );
}

void GUIMoneyBar::draw( Window& window )
{
    window.draw( this->bar );
    window.draw( this->moneyText );
}