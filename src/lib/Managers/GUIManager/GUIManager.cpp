#include "GUIManager.h"
#include "pch.h"


#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "SceneManager.h"

#include "ShieldSystem.h"
#include "WeaponsSystem.h"

#include "NotifySystem.h"

GUIManager::GUIManager( entt::registry& registry, sf::Event& event, std::vector< Quest >& quests )
    : registry( registry ), event( event ), skillTreeGUI( registry )
{
    m_widgets.reserve( 7 );

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

    GUIQuickMenu quickMenu( registry );

    quickMenu.setOnOpenCallback( [ this ]()
    {
        slowMotionFromGUI = true;
        quickMenuActive = true;
    });
    quickMenu.setOnCloseCallback( [ this ]( int tile )
    {
        slowMotionFromGUI = false;
        onQuickMenuClose( tile );
        quickMenuActive = false;
    });

    m_widgets.emplace_back( GUIEnergyBar( registry ) );
    m_widgets.emplace_back( GUIMinimap( registry, quests ) );
    m_widgets.emplace_back( GUIMoneyBar( registry ) );
    m_widgets.emplace_back( GUIExpInfo( registry ) );
    m_widgets.emplace_back( std::move( journal ) );
    m_widgets.emplace_back( std::move( quickMenu ) );
    m_widgets.emplace_back( GUIWeaponTile( registry ) );
    m_widgets.emplace_back( GUIShieldTile( registry ) );
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

        for ( auto& widget : m_widgets )
        {
            widget.update( deltaTime, this->event, mouseState );
        }

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

void GUIManager::onQuickMenuClose( int selectedTile )
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
}