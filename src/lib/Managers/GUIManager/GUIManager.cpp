#include "GUIManager.h"
#include "pch.h"


#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "SceneManager.h"

#include "ShieldSystem.h"
#include "WeaponsSystem.h"

#include "NotifySystem.h"

GUIManager::GUIManager( entt::registry& registry, sf::Event& event, std::vector< Quest >& quests )
    : registry( registry ), event( event ), quickMenu( registry ), skillTreeGUI( registry ),
      weaponTile( registry ), shieldTile( registry )
{
    m_widgets.reserve( 5 );

    GUIJournal journal( quests );

    journal.setOnOpenCallback( [ this ]()
    {
        pauseFromGUI = true;
    } );
    journal.setOnCloseCallback( [ this ]()
    {
        pauseFromGUI = false;
        quitTimer = 0.0F;
    } );

    m_widgets.emplace_back( GUIEnergyBar( registry ) );
    m_widgets.emplace_back( GUIMinimap( registry, quests ) );
    m_widgets.emplace_back( GUIMoneyBar( registry ) );
    m_widgets.emplace_back( GUIExpInfo( registry ) );
    m_widgets.emplace_back( std::move( journal ) );

}

void GUIManager::processInput( const InputContext& inputContext )
{
    for ( auto& widget : m_widgets )
    {
        widget.handleEvent( inputContext );
    }
}

void GUIManager::update( sf::Time deltaTime, const Mouse::MouseState& mouseState )
{
    if ( SceneManager::getInstance().getCurrentScene() == Scene::Game )
    {
        if ( !SceneManager::getInstance().isGameStarted() )
        {
            this->weaponTile.clear();
            this->shieldTile.clear();
            SceneManager::getInstance().setGameStarted( true );
        }
        else
        {
            NotifySystem::update( deltaTime, mouseState );
        }

        if ( this->quitTimer < 0.2f )
        {
            this->readyToQuit = false;
            this->quitTimer += deltaTime.asSeconds();
        }
        else
        {
            this->readyToQuit = true;
        }

        if ( this->quickMenuActive )
        {
            this->quickMenu.update( mouseState );
        }

        for ( auto& widget : m_widgets )
        {
            widget.update( deltaTime, this->event, mouseState );
        }

        this->weaponTile.update();
        this->shieldTile.update();
        

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) && !pauseFromGUI && this->readyToQuit )
        {
            SceneManager::getInstance().setCurrentScene( Scene::MainMenu );
        }
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::SkillTree )
    {
        this->skillTreeGUI.update( deltaTime, mouseState );
        this->quitTimer = 0.F;
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::MainMenu )
    {
        this->mainMenu.update( mouseState, deltaTime );
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::Settings )
    {
        this->settings.update( mouseState, deltaTime );
    }
}

void GUIManager::draw( Window& window )
{
    if ( SceneManager::getInstance().getCurrentScene() == Scene::Game )
    {
        for ( auto& widget : m_widgets )
        {
            widget.draw( window );
        }
        
        this->shieldTile.draw( window );
        this->weaponTile.draw( window );

        if ( this->quickMenuActive )
        {
            this->quickMenu.draw( window );
        }
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::SkillTree )
    {
        this->skillTreeGUI.draw( window );
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::MainMenu )
    {
        this->mainMenu.draw( window );
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::Settings )
    {
        this->settings.draw( window );
    }
}

void GUIManager::toggleQuickMenu( bool value )
{
    this->quickMenuActive = value;
    // TODO: temporary solution for changing weapon
    if ( !this->quickMenuActive )
    {
        switch ( this->quickMenu.getSelectedTile() )
        {
        case 1:
            WeaponsSystem::changeWeapon( this->registry, Weapons::RedWeapon );
            this->weaponTile.setWeaponTexture( "red_weapon" );
            break;
        case 2:
            WeaponsSystem::changeWeapon( this->registry, Weapons::BlueWeapon );
            this->weaponTile.setWeaponTexture( "blue_weapon" );
            break;
        case 3:
            ShieldSystem::changeShield( this->registry, basicShield );
            this->shieldTile.setShieldTexture( "basic_shield_ico" );
            break;
        case 4:
            ShieldSystem::changeShield( this->registry, advancedShield );
            this->shieldTile.setShieldTexture( "advanced_shield_ico" );
            break;
        case 5:
            WeaponsSystem::changeWeapon( this->registry, Weapons::QuadWeapon );
            this->weaponTile.setWeaponTexture( "quad_weapon" );
            break;
        case 9:
            SceneManager::getInstance().setCurrentScene( Scene::SkillTree );
            break;
        default:
            break;
        }
    }
}