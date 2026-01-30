#include "Game.h"
#include "pch.h"

#include "ConfigManager.hpp"
#include "SceneManager.h"
#include "TextureManager.h"

Game::Game()
    : event( sf::Event::Closed{} ), systemManager( m_window.getWindow(), this->registry, this->event ),
      guiManager( m_window.getWindow(), this->registry, this->event, this->systemManager.getQuests() ),
      cursor( sf::Cursor::createFromSystem( sf::Cursor::Type::Arrow ).value() )
{
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

    if ( !this->cursor.createFromPixels( this->cursorTexture.copyToImage().getPixelsPtr(),
                                       this->cursorTexture.getSize(), hotSpot ) )
    {
        std::cerr << "Failed to load cursor texture" << '\n';
    }

    m_window.setMouseCursor( this->cursor );
}

void Game::run()
{
    while ( m_window.isOpen() )
    {
        this->processEvents();
        this->update( this->clock.restart() );
        this->render();
    }
}

void Game::processEvents()
{
    while ( const auto event = m_window.pollEvent() )
    {
        this->event = *event; 

        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->code == sf::Keyboard::Key::Tab && !this->guiManager.pause())
            {
                this->systemManager.enableSlowMotion();
                this->guiManager.toggleQuickMenu(true);
            }

            if (keyPressed->code == sf::Keyboard::Key::Grave)
            {
                this->systemManager.debugMode = !this->systemManager.debugMode;
            }
        }

        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            if (keyReleased->code == sf::Keyboard::Key::Tab)
            {
                this->systemManager.disableSlowMotion();
                this->guiManager.toggleQuickMenu(false);
            }
        }
    }

    this->guiManager.processInput();

    if (!this->guiManager.pause())
    {
        this->systemManager.executeEventSystems();
    }
}

void Game::update( sf::Time deltaTime )
{
    this->guiManager.update( deltaTime, m_window.getMouseState() );
    if ( !this->guiManager.pause() )
    {
        this->systemManager.executeUpdateSystems( deltaTime, m_window );
    }
}

void Game::render()
{
    m_window.clear();

    this->systemManager.executeRenderSystems( m_window );
    this->guiManager.draw( m_window );

    m_window.display();
}