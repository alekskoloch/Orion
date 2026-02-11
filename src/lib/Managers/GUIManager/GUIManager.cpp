#include "GUIManager.h"
#include "pch.h"


#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "SceneManager.h"

#include "NotifySystem.h"

GUIManager::GUIManager( entt::registry& registry, sf::Event& event, std::vector< Quest >& quests, GameState* gameState )
    : registry( registry ), event( event ), skillTreeGUI( registry ), m_gameState( gameState ), m_mainMenu( gameState )
{
    // m_widgets.reserve( 7 );

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
    quickMenu.setOnCloseCallback( [ this ]()
    {
        slowMotionFromGUI = false;
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
    if ( *m_gameState == GameState::Menu )
    {
        m_mainMenu.handleEvent( inputContext );
    }
    else if ( *m_gameState == GameState::Game )
    {
        for ( auto& widget : m_widgets )
        {
            widget.handleEvent( inputContext );
        }
    }
    else if ( *m_gameState == GameState::Player )
    {
        // TODO: PlayerPanel
    }
}

void GUIManager::update( sf::Time deltaTime, const Mouse::MouseState& mouseState )
{
    if ( *m_gameState == GameState::Menu )
    {
        m_mainMenu.update( deltaTime );
    }
    else if ( *m_gameState == GameState::Game )
    {
        for ( auto& widget : m_widgets )
        {
            widget.update( deltaTime );
        }

        NotifySystem::update( deltaTime, mouseState );
    }
    else if ( *m_gameState == GameState::Player )
    {
        // TODO: PlayerPanel
    }

    // if ( SceneManager::getInstance().getCurrentScene() == Scene::Game )
    // {
    //     if ( !SceneManager::getInstance().isGameStarted() )
    //     {
    //         SceneManager::getInstance().setGameStarted( true );
    //     }
    //     else
    //     {
    //         NotifySystem::update( deltaTime, mouseState );
    //     }

    //     if ( this->quitTimer < 0.2f )
    //     {
    //         this->readyToQuit = false;
    //         this->quitTimer += deltaTime.asSeconds();
    //     }
    //     else
    //     {
    //         this->readyToQuit = true;
    //     }

    //     for ( auto& widget : m_widgets )
    //     {
    //         widget.update( deltaTime, this->event, mouseState );
    //     }

    //     if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) && !pauseFromGUI && this->readyToQuit )
    //     {
    //         SceneManager::getInstance().setCurrentScene( Scene::MainMenu );
    //     }
    // }
    // else if ( SceneManager::getInstance().getCurrentScene() == Scene::SkillTree )
    // {
    //     this->skillTreeGUI.update( deltaTime, mouseState );
    //     this->quitTimer = 0.F;
    // }
    // else if ( SceneManager::getInstance().getCurrentScene() == Scene::MainMenu )
    // {
    //     this->mainMenu.update( mouseState, deltaTime );
    // }
    // else if ( SceneManager::getInstance().getCurrentScene() == Scene::Settings )
    // {
    //     this->settings.update( mouseState, deltaTime );
    // }
}

void GUIManager::draw( Window& window )
{
    if ( *m_gameState == GameState::Menu )
    {
        m_mainMenu.draw( window );
    }
    else if ( *m_gameState == GameState::Game )
    {
        for ( auto& widget : m_widgets )
        {
            widget.draw( window );
        }
    }
    else if ( *m_gameState == GameState::Player )
    {
        // TODO: PlayerPanel
    }

    // if ( SceneManager::getInstance().getCurrentScene() == Scene::Game )
    // {
    //     for ( auto& widget : m_widgets )
    //     {
    //         widget.draw( window );
    //     }
    // }
    // else if ( SceneManager::getInstance().getCurrentScene() == Scene::SkillTree )
    // {
    //     this->skillTreeGUI.draw( window );
    // }
    // else if ( SceneManager::getInstance().getCurrentScene() == Scene::MainMenu )
    // {
    //     this->mainMenu.draw( window );
    // }
    // else if ( SceneManager::getInstance().getCurrentScene() == Scene::Settings )
    // {
    //     this->settings.draw( window );
    // }
}