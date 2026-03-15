#include "MainMenu.hpp"

#include <SFML/System/Time.hpp>
#include "InputContext.hpp"
#include "TabContentBuilder.hpp"
#include "Window.hpp"
#include "ConfigManager.hpp"
#include "TabWindow.hpp"

namespace
{
    constexpr float BodyWidthRatio = 0.95f;
    constexpr float BodyHeightRatio = 0.55f;
    constexpr float BodyPosYRatio = 0.56f;
    constexpr float BackButtonPosYRatio = 0.90f;
    constexpr float ContentStartOffset = -150.0f; 
    constexpr float ContentSpacing = 180.0f;
}

MainMenu::MainMenu( GameState* gameState ) : m_gameState( gameState )
{
    initMainMenu();
    initLoadGame();
    initSettings();
}

void MainMenu::handleEvent( const InputContext& inputContext )
{
    std::vector< MenuElement >* elemetnsToHandle{ nullptr };
    switch ( m_menuState )
    {
    case MenuState::MainMenu:
        elemetnsToHandle = &m_mainMenuElements;
        break;
    case MenuState::LoadGame:
        elemetnsToHandle = &m_loadGameElements;
        break;
    case MenuState::Settings:
        elemetnsToHandle = &m_settingsElements;
        break;
    }

    if ( elemetnsToHandle )
    {
        for ( auto& element : *elemetnsToHandle )
        {
            element.handleEvent( inputContext );
        }
    }
}

void MainMenu::update( const sf::Time deltaTime )
{
    std::vector< MenuElement >* elemetnsToHandle{ nullptr };
    switch ( m_menuState )
    {
    case MenuState::MainMenu:
        elemetnsToHandle = &m_mainMenuElements;
        break;
    case MenuState::LoadGame:
        elemetnsToHandle = &m_loadGameElements;
        break;
    case MenuState::Settings:
        elemetnsToHandle = &m_settingsElements;
        break;
    }

    if ( elemetnsToHandle )
    {
        for ( auto& element : *elemetnsToHandle )
        {
            element.update( deltaTime );
        }
    }
}

void MainMenu::draw( Window& window )
{
    std::vector< MenuElement >* elemetnsToHandle{ nullptr };
    switch ( m_menuState )
    {
    case MenuState::MainMenu:
        elemetnsToHandle = &m_mainMenuElements;
        break;
    case MenuState::LoadGame:
        elemetnsToHandle = &m_loadGameElements;
        break;
    case MenuState::Settings:
        elemetnsToHandle = &m_settingsElements;
        break;
    }

    if ( elemetnsToHandle )
    {
        for ( auto& element : *elemetnsToHandle )
        {
            element.draw( window );
        }
    }
}

void MainMenu::initMainMenu()
{
    constexpr auto spacingBetweenButtons = 260.F;

    const auto& config = ConfigManager::getInstance();

    const auto posX = static_cast< float >( config.getScreenWidth() ) * 0.83F;
    const auto startY = static_cast< float >( config.getScreenHeight() ) * 0.33F;
    float spacing = spacingBetweenButtons * config.getScale();

    const auto titleY = static_cast< float >( config.getScreenHeight() ) * 0.30F;
    const auto titlePosX = static_cast< float >( config.getScreenWidth() ) * 0.30F;
    
    m_mainMenuElements.emplace_back( Title( "Orion", sf::Vector2f{ titlePosX, titleY }, 550 * config.getScale() ) );

    m_mainMenuElements.emplace_back( Button( "Continue", sf::Vector2f{ posX, startY }, [ this ]() { *m_gameState = GameState::Game; } ) );
    m_mainMenuElements.emplace_back( Button( "Start Game", sf::Vector2f{ posX, startY + spacing }, [ this ]() { *m_gameState = GameState::Game; } ) );
    m_mainMenuElements.emplace_back( Button( "Load Game", sf::Vector2f{ posX, startY + spacing * 2 }, [ this ]() { m_menuState = MenuState::LoadGame; } ) );
    m_mainMenuElements.emplace_back( Button( "Settings", sf::Vector2f{ posX, startY + spacing * 3 }, [ this ]() { m_menuState = MenuState::Settings; } ) );
    m_mainMenuElements.emplace_back( Button( "Quit", sf::Vector2f{ posX, startY + spacing * 4 }, [ this ]() { *m_gameState = GameState::Quit; } ) );
}

void MainMenu::initLoadGame()
{
    constexpr auto spacingBetweenButtons = 260.F;
    const auto& config = ConfigManager::getInstance();

    const auto mainMenuButtonPosX = static_cast< float >( config.getScreenWidth() ) * 0.20F;
    const auto loadButtonsPosX = static_cast< float >( config.getScreenWidth() ) * 0.50F;
    const auto startY = static_cast< float >( config.getScreenHeight() ) * 0.33F;
    float spacing = spacingBetweenButtons * config.getScale();

    const auto titleY = static_cast< float >( config.getScreenHeight() ) * 0.15F;
    const auto titlePosX = static_cast< float >( config.getScreenWidth() ) * 0.50F;
    
    m_loadGameElements.emplace_back( Title( "Load Game", sf::Vector2f{ titlePosX, titleY }, 350 * config.getScale() ) );
    m_loadGameElements.emplace_back( Button( "Load 1", sf::Vector2f{ loadButtonsPosX, startY }, [ this ]() {}, Alignment::Center ) );
    m_loadGameElements.emplace_back( Button( "Load 2", sf::Vector2f{ loadButtonsPosX, startY + spacing }, [ this ]() {}, Alignment::Center ) );
    m_loadGameElements.emplace_back( Button( "Load 3", sf::Vector2f{ loadButtonsPosX, startY + spacing * 2 }, [ this ]() {}, Alignment::Center ) );
    m_loadGameElements.emplace_back( Button( "Load 4", sf::Vector2f{ loadButtonsPosX, startY + spacing * 3 }, [ this ]() {}, Alignment::Center ) );
    m_loadGameElements.emplace_back( Button( "Main Menu", sf::Vector2f{ mainMenuButtonPosX, startY + spacing * 4 }, [ this ]() { m_menuState = MenuState::MainMenu; }, Alignment::Left ) );
}

void MainMenu::initSettings()
{
    const auto& config = ConfigManager::getInstance();
    
    const auto screenW = static_cast< float >( config.getScreenWidth() );
    const auto screenH = static_cast< float >( config.getScreenHeight() );

    const auto titlePosX = screenW * 0.50F;
    const auto titlePosY = screenH * 0.15F;
    
    m_settingsElements.emplace_back( Title( "Settings", sf::Vector2f{ titlePosX, titlePosY }, 350 * config.getScale() ) );

    sf::Vector2f bodySize{ screenW * BodyWidthRatio, screenH * BodyHeightRatio };
    sf::Vector2f bodyPos{ screenW * 0.5f, screenH * BodyPosYRatio };
    
    // Punkt startowy dla elementów wewnątrz zakładki (np. odrobinę wyżej niż środek)
    sf::Vector2f contentStartPos = bodyPos;
    contentStartPos.y += ContentStartOffset * config.getScale();
    
    float spacing = ContentSpacing * config.getScale();
    auto noOp = [](){};

    TabWindow tabWindow( bodySize, bodyPos );
    
    // UŻYCIE BUILDERA:
    
    tabWindow
        .addTab( SettingsTab::General, "General", 
            TabContentBuilder( contentStartPos, spacing )
                .addTitle( "Game Config" )
                .addButton( "Reset Config", noOp )
                .addButton( "Clear Cache", noOp )
                .build() 
        )
        .addTab( SettingsTab::Graphics, "Graphics", 
            TabContentBuilder( contentStartPos, spacing )
                .addButton( "Resolution", noOp )
                .addButton( "Toggle VSync", noOp )
                .addButton( "Quality: High", noOp )
                .build()
        )
        .addTab( SettingsTab::Audio, "Audio",
            TabContentBuilder( contentStartPos, spacing )
                .addSlider( []( float val ){ /* TODO: implement volume change */ }, 0.0f, 100.0f, 80.0f )
                .addSlider( []( float val ){ /* TODO: implement volume change */ }, 0.0f, 100.0f, 70.0f )
                .build()
        )
        .addTab( SettingsTab::Controls, "Controls", 
             TabContentBuilder( contentStartPos, spacing )
                .addButton( "Rebind Keys", noOp )
                .build()
        );

    m_settingsElements.emplace_back( std::move( tabWindow ) );

    const auto backBtnX = screenW * 0.11F;
    const auto backBtnY = screenH * BackButtonPosYRatio;

    m_settingsElements.emplace_back( 
        Button( "Back", sf::Vector2f{ backBtnX, backBtnY }, 
        [ this ]() { m_menuState = MenuState::MainMenu; }, 
        Alignment::Left ) 
    );
}