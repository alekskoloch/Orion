#include "Game.h"
#include "pch.h"

#include "ConfigManager.hpp"
#include "TextureManager.h"

#include "CameraSystem.h"

Game::Game()
    : event( sf::Event::Closed{} ), systemManager( this->registry, this->event, &m_gameState ),
      guiManager( this->registry, this->event, this->systemManager.getQuests(), &m_gameState ),
      cursor( sf::Cursor::createFromSystem( sf::Cursor::Type::Arrow ).value() )
{
    this->loadCursor();
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

    sf::Image cursorImage = this->cursorTexture.copyToImage();

    auto loadedCursor = sf::Cursor::createFromPixels( cursorImage.getPixelsPtr(), cursorImage.getSize(), hotSpot );

    if ( loadedCursor.has_value() )
    {
        this->cursor = std::move( loadedCursor.value() );
        m_window.setMouseCursor( this->cursor );
    }
    else
    {
        std::cerr << "Failed to load cursor texture" << '\n';
    }
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

        if ( event->is< sf::Event::Closed >() )
        {
            m_window.close();
        }

        if ( m_gameState == GameState::Quit )
        {
            m_window.close();
        }

        if ( const auto* keyPressed = event->getIf< sf::Event::KeyPressed >() )
        {
            if ( keyPressed->code == sf::Keyboard::Key::Grave )
            {
                this->systemManager.debugMode = !this->systemManager.debugMode;
            }
        }

        auto mouseState = m_window.getMouseState();
        InputContext inputContext( *event, mouseState );
        this->guiManager.processInput( inputContext );
    }

    guiManager.slowMotion() ? this->systemManager.enableSlowMotion() : this->systemManager.disableSlowMotion();

    if ( !this->guiManager.pause() )
    {
        this->systemManager.executeEventSystems();
    }
}

void Game::update( sf::Time deltaTime )
{
    auto rawMouseState = m_window.getMouseState();
    sf::Vector2i mousePixelPos( static_cast< int >( rawMouseState.screenPosition.x ),
                                static_cast< int >( rawMouseState.screenPosition.y ) );

    auto uiMouseState = rawMouseState;
    sf::Vector2f uiWorldPos;

    if ( false /* TODO: SkillTree scene */ )
    {
        uiWorldPos = m_window.getWindow().mapPixelToCoords( mousePixelPos, this->guiManager.getSkillTreeView() );
    }
    else
    {
        uiWorldPos = m_window.getWindow().mapPixelToCoords( mousePixelPos, m_window.getWindow().getDefaultView() );
    }

    uiMouseState.worldPosition = { .x = uiWorldPos.x, .y = uiWorldPos.y };

    this->guiManager.update( deltaTime, uiMouseState );

    if ( !this->guiManager.pause() )
    {
        CameraSystem::updateZoomFactor( deltaTime );
        CameraSystem::updateCamera( m_gameView, this->registry );

        m_window.setView( m_gameView );

        auto gameMouseState = rawMouseState;
        sf::Vector2f gameWorldPos = m_window.getWindow().mapPixelToCoords( mousePixelPos, m_gameView );
        gameMouseState.worldPosition = { gameWorldPos.x, gameWorldPos.y };

        this->systemManager.executeUpdateSystems( deltaTime, gameMouseState );
    }
}

void Game::render()
{
    m_window.clear();

    if ( m_gameState == GameState::Game )
    {
        m_window.setView( m_gameView );

        this->systemManager.executeRenderSystems( m_window );
    }

    m_window.setDefaultView();

    if ( m_gameState == GameState::Game )
    {
        NotifySystem::draw( m_window );
    }

    this->guiManager.draw( m_window );

    m_window.display();
}