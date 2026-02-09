#include "GUIQuickMenu.h"
#include "pch.h"


#include "InputData.hpp"
#include "Window.hpp"

#include "../Managers/SceneManager/SceneManager.h"

#include "../Systems/EnergySystem/EnergySystem.h"
#include "../Systems/WeaponSystem/WeaponsSystem.h"
#include "../Systems/ShieldSystem/ShieldSystem.h"


const uint8_t TILES_NUMBER = 8;
const float RADIUS = 360.f;
const float ANGLE_INCREMENT = 45.f;

GUIQuickMenu::GUIQuickMenu( entt::registry& registry ) : registry( registry ) { this->initializeQuickMenu(); }

void GUIQuickMenu::setOnOpenCallback( std::function< void() > callback )
{
    m_onOpenCallback = callback;
}

void GUIQuickMenu::setOnCloseCallback( std::function< void() > callback )
{
    m_onCloseCallback = callback;
}

void GUIQuickMenu::onOpen()
{
    if ( m_onOpenCallback )
    {
        m_onOpenCallback();
    }
}

void GUIQuickMenu::onClose()
{
    switch ( selectedTile )
    {
    case 1:
        WeaponsSystem::changeWeapon( this->registry, Weapons::RedWeapon );
        break;
    case 2:
        WeaponsSystem::changeWeapon( this->registry, Weapons::BlueWeapon );
        break;
    case 3:
        ShieldSystem::changeShield( this->registry, basicShield );
        break;
    case 4:
        ShieldSystem::changeShield( this->registry, advancedShield );
        break;
    case 5:
        WeaponsSystem::changeWeapon( this->registry, Weapons::QuadWeapon );
        break;
    case 9:
        SceneManager::getInstance().setCurrentScene( Scene::SkillTree );
        break;
    default:
        break;
    }

    if ( m_onCloseCallback )
    {
        m_onCloseCallback();
    }
}

void GUIQuickMenu::handleEvent( const InputContext& inputContext )
{
    const auto& event = inputContext.getEvent();

    if ( const auto* keyPressed = event.getIf< sf::Event::KeyPressed >() )
    {
        if ( keyPressed->code == sf::Keyboard::Key::Tab && !this->isOpen )
        {
            this->isOpen = true;
            onOpen();
        }
    }

    if ( const auto* keyReleased = event.getIf< sf::Event::KeyReleased >() )
    {
        if ( keyReleased->code == sf::Keyboard::Key::Tab && this->isOpen )
        {
            this->isOpen = false;
            onClose();
            this->selectedTile = 0;
        }
    }

    if ( isOpen )
    {
        const auto& mousePosition{
            sf::Vector2i{ inputContext.getMouseState().screenPosition.x, inputContext.getMouseState().screenPosition.y } };

        for ( int i = 0; i < TILES_NUMBER; i++ )
        {
            if ( utils::isMouseOverSprite( this->quickMenuTiles[ i ], mousePosition ) )
            {
                if ( this->selectedTile != i + 1 )
                {
                    SoundManager::getInstance().playSound( "MouseHover" );
                }

                this->quickMenuTiles[ i ].setTexture( TextureManager::getInstance().getTexture( "ACTIVE_TILE" ) );
                this->selectedTile = i + 1;
            }
            else
            {
                this->quickMenuTiles[ i ].setTexture( TextureManager::getInstance().getTexture( "INACTIVE_TILE" ) );
                if ( this->selectedTile == i + 1 )
                {
                    this->selectedTile = 0;
                }
            }
        }

        if ( utils::isMouseOverSprite( this->quickMenuTiles[ TILES_NUMBER ], mousePosition ) )
        {
            if ( this->selectedTile != TILES_NUMBER + 1 )
            {
                SoundManager::getInstance().playSound( "MouseHover" );
            }

            this->quickMenuTiles[ TILES_NUMBER ].setTexture( TextureManager::getInstance().getTexture( "ACTIVE_MIDDLE_TILE" ) );
            this->selectedTile = TILES_NUMBER + 1;
        }
        else
        {
            this->quickMenuTiles[ TILES_NUMBER ].setTexture( TextureManager::getInstance().getTexture( "INACTIVE_MIDDLE_TILE" ) );
            if ( this->selectedTile == 9 )
            {
                this->selectedTile = TILES_NUMBER + 1;
            }
        }
    }
}

void GUIQuickMenu::draw( Window& window )
{
    if( !isOpen )
    {
        return;
    }

    for ( auto& tile : this->quickMenuTiles )
    {
        window.draw( tile );
    }

    for ( auto& icon : this->quickMenuIcons )
    {
        window.draw( icon );
    }
}

void GUIQuickMenu::initializeQuickMenu()
{
    TextureManager::getInstance().loadTexture( "ACTIVE_TILE", ASSETS_PATH + std::string( "tileActive.png" ) );
    TextureManager::getInstance().loadTexture( "INACTIVE_TILE", ASSETS_PATH + std::string( "tileInactive.png" ) );
    TextureManager::getInstance().loadTexture( "ACTIVE_MIDDLE_TILE", ASSETS_PATH + std::string( "middleTileActive.png" ) );
    TextureManager::getInstance().loadTexture( "INACTIVE_MIDDLE_TILE", ASSETS_PATH + std::string( "middleTileInactive.png" ) );
    TextureManager::getInstance().loadTexture( "EMPTY", ASSETS_PATH + std::string( "empty.png" ) );

    // TODO: this is a temporary solution for loading bullet ico textures
    TextureManager::getInstance().loadTexture( "red_weapon", ASSETS_PATH + std::string( "red_weapon.png" ) );
    TextureManager::getInstance().loadTexture( "blue_weapon", ASSETS_PATH + std::string( "blue_weapon.png" ) );
    TextureManager::getInstance().loadTexture( "quad_weapon", ASSETS_PATH + std::string( "quad_weapon.png" ) );
    TextureManager::getInstance().loadTexture( "basic_shield_ico", ASSETS_PATH + std::string( "basic_shield_ico.png" ) );
    TextureManager::getInstance().loadTexture( "advanced_shield_ico", ASSETS_PATH + std::string( "advanced_shield_ico.png" ) );

    this->quickMenuTiles.clear();
    this->quickMenuTiles.reserve( TILES_NUMBER + 1 );

    this->quickMenuIcons.clear();
    this->quickMenuIcons.reserve( TILES_NUMBER + 1 );

    for ( int i = 0; i < TILES_NUMBER + 1; i++ )
    {
        this->quickMenuTiles.push_back( CreateSprite( "INACTIVE_TILE" ) );
        this->quickMenuIcons.push_back( CreateSprite( "Empty" ) );
    }

    const auto windowWidth = ConfigManager::getInstance().getScreenWidth();
    const auto windowHeight = ConfigManager::getInstance().getScreenHeight();

    for ( int i = 0; i < TILES_NUMBER; i++ )
    {
        this->quickMenuTiles[ i ] = CreateSprite( "INACTIVE_TILE" );
        this->quickMenuTiles[ i ].setScale(
            sf::Vector2f{ ConfigManager::getInstance().getScale(), ConfigManager::getInstance().getScale() } );
        this->quickMenuTiles[ i ].setPosition( sf::Vector2f{
            static_cast< float >( ( windowWidth / 2 ) + ( std::sin( ( i * ANGLE_INCREMENT ) * M_PI / 180.f ) *
                                                          ( RADIUS * ConfigManager::getInstance().getScale() ) ) ),
            static_cast< float >( ( windowHeight / 2 ) - ( std::cos( ( i * ANGLE_INCREMENT ) * M_PI / 180.f ) *
                                                           ( RADIUS * ConfigManager::getInstance().getScale() ) ) ) } );

        using namespace sf::Literals;
        const sf::Angle ANGLE_INCREMENT = 45_deg;

        sf::Angle angle = i * ANGLE_INCREMENT;
        this->quickMenuTiles[ i ].setRotation( angle );

        // TODO: this is a temporary solution for loading bullet ico textures
        if ( i == 0 )
        {
            this->quickMenuIcons[ i ] = CreateSprite( "red_weapon" );
        }
        else if ( i == 1 )
        {
            this->quickMenuIcons[ i ] = CreateSprite( "blue_weapon" );
        }
        else if ( i == 2 )
        {
            this->quickMenuIcons[ i ] = CreateSprite( "basic_shield_ico" );
        }
        else if ( i == 3 )
        {
            this->quickMenuIcons[ i ] = CreateSprite( "advanced_shield_ico" );
        }
        else if ( i == 4 )
        {
            this->quickMenuIcons[ i ] = CreateSprite( "quad_weapon" );
        }

        this->quickMenuIcons[ i ].setPosition( this->quickMenuTiles[ i ].getPosition() );
        this->quickMenuIcons[ i ].setScale(
            sf::Vector2f{ ConfigManager::getInstance().getScale(), ConfigManager::getInstance().getScale() } );
    }

    this->quickMenuTiles[ TILES_NUMBER ] = CreateSprite( "INACTIVE_MIDDLE_TILE" );
    this->quickMenuTiles[ TILES_NUMBER ].setScale(
        sf::Vector2f{ ConfigManager::getInstance().getScale(), ConfigManager::getInstance().getScale() } );
    this->quickMenuTiles[ TILES_NUMBER ].setPosition(
        sf::Vector2f{ ( windowWidth / 2.f ), ( windowHeight / 2.f ) } );
}