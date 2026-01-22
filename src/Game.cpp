#include "Game.h"
#include "pch.h"

#include "ConfigManager.hpp"
#include "SceneManager.h"
#include "TextureManager.h"

Game::Game()
    : window( sf::VideoMode( ConfigManager::getInstance().getScreenWidth(),
                             ConfigManager::getInstance().getScreenHeight() ),
              "Orion", ConfigManager::getInstance().getWindowStyle() ),
      systemManager( this->window, this->registry, this->event ),
      guiManager( this->window, this->registry, this->event, this->systemManager.getQuests() )
{
    this->window.setFramerateLimit( ConfigManager::getInstance().getFrameRateLimit() );

    this->loadCursor();

    SceneManager::getInstance().setCurrentScene( Scene::MainMenu );
}

void Game::loadCursor()
{
    sf::Vector2u hotSpot;

    constexpr auto cursorMinThreshold = 0.25F;
    constexpr auto cursorMidThreshold = 0.5F;

    constexpr auto cursorSmallSize = 6;
    constexpr auto cursorMediumSize = 12;
    constexpr auto cursorLargeSize = 25;

    if ( ConfigManager::getInstance().getScale() <= cursorMinThreshold )
    {
        this->cursorTexture = TextureManager::getInstance().getTexture( "CursorSmall" );
        hotSpot = { cursorSmallSize, cursorSmallSize };
    }
    else if ( ConfigManager::getInstance().getScale() <= cursorMidThreshold )
    {
        this->cursorTexture = TextureManager::getInstance().getTexture( "CursorMedium" );
        hotSpot = { cursorMediumSize, cursorMediumSize };
    }
    else
    {
        this->cursorTexture = TextureManager::getInstance().getTexture( "CursorLarge" );
        hotSpot = { cursorLargeSize, cursorLargeSize };
    }

    if ( !this->cursor.loadFromPixels( this->cursorTexture.copyToImage().getPixelsPtr(),
                                       this->cursorTexture.getSize(), hotSpot ) )
    {
        std::cerr << "Failed to load cursor texture" << '\n';
    }

    this->window.setMouseCursor( this->cursor );
}

void Game::run()
{
    while ( window.isOpen() )
    {
        this->processEvents();
        this->update( this->clock.restart() );
        this->render();
    }
}

void Game::processEvents()
{
    this->event = sf::Event();

    while ( window.pollEvent( event ) )
    {
        if ( event.type == sf::Event::Closed )
        {
            window.close();
        }

        if ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab &&
             !this->guiManager.pause() )
        {
            this->systemManager.enableSlowMotion();
            this->guiManager.toggleQuickMenu( true );
        }
        if ( event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Tab )
        {
            this->systemManager.disableSlowMotion();
            this->guiManager.toggleQuickMenu( false );
        }

        if ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tilde )
        {
            this->systemManager.debugMode = !this->systemManager.debugMode;
        }
    }

    this->guiManager.processInput();

    if ( !this->guiManager.pause() )
    {
        this->systemManager.executeEventSystems();
    }
}

void Game::update( sf::Time deltaTime )
{
    this->guiManager.update( deltaTime );
    if ( !this->guiManager.pause() )
    {
        this->systemManager.executeUpdateSystems( deltaTime );
    }
}

void Game::render()
{
    this->window.clear();

    this->systemManager.executeRenderSystems();
    this->guiManager.draw();

    this->window.display();
}