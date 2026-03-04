#include "MainMenu.hpp"

#include <SFML/System/Time.hpp>

#include "InputContext.hpp"
#include "Window.hpp"

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

    for ( auto& element : *elemetnsToHandle )
    {
        element.handleEvent( inputContext );
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

    for ( auto& element : *elemetnsToHandle )
    {
        element.update( deltaTime );
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

    for ( auto& element : *elemetnsToHandle )
    {
        element.draw( window );
    }
}

void MainMenu::initMainMenu()
{
    constexpr auto margin = 200.F;
    constexpr auto spacingBetweenButtons = 260.F;

    const auto& config = ConfigManager::getInstance();

    const auto posX = static_cast< float >( config.getScreenWidth() ) * 0.83F;
    const auto startY = static_cast< float >( config.getScreenHeight() ) * 0.33F;

    float spacing = spacingBetweenButtons * config.getScale();

    const auto titleY = static_cast< float >( config.getScreenHeight() ) * 0.30F;

    const auto titlePosX = static_cast< float >( config.getScreenWidth() ) * 0.30F;
    m_mainMenuElements.emplace_back( Title( "Orion", sf::Vector2f{ titlePosX, titleY }, 550 * config.getScale() ) );

    m_mainMenuElements.emplace_back( Button( "Continue", sf::Vector2f{ posX, startY }, [ this ]() { *m_gameState = GameState::Game; } ) );

    m_mainMenuElements.emplace_back(
        Button( "Start Game", sf::Vector2f{ posX, startY + spacing }, [ this ]() { *m_gameState = GameState::Game; } ) );

    m_mainMenuElements.emplace_back( Button( "Load Game", sf::Vector2f{ posX, startY + spacing * 2 }, [ this ]() { m_menuState = MenuState::LoadGame; } ) );

    m_mainMenuElements.emplace_back( Button( "Settings", sf::Vector2f{ posX, startY + spacing * 3 }, [ this ]() { m_menuState = MenuState::Settings; } ) );

    m_mainMenuElements.emplace_back(
        Button( "Quit", sf::Vector2f{ posX, startY + spacing * 4 }, [ this ]() { *m_gameState = GameState::Quit; } ) );
}

void MainMenu::initLoadGame()
{
    constexpr auto margin = 200.F;
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

    m_loadGameElements.emplace_back(
        Button( "Load 2", sf::Vector2f{ loadButtonsPosX, startY + spacing }, [ this ]() {}, Alignment::Center ) );

    m_loadGameElements.emplace_back( Button( "Load 3", sf::Vector2f{ loadButtonsPosX, startY + spacing * 2 }, [ this ]() {}, Alignment::Center ) );

    m_loadGameElements.emplace_back( Button( "Load 4", sf::Vector2f{ loadButtonsPosX, startY + spacing * 3 }, [ this ]() {}, Alignment::Center ) );

    m_loadGameElements.emplace_back(
        Button( "Main Menu", sf::Vector2f{ mainMenuButtonPosX, startY + spacing * 4 }, [ this ]() { m_menuState = MenuState::MainMenu; }, Alignment::Left ) );
}

void MainMenu::initSettings()
{
    constexpr auto margin = 200.F;
    constexpr auto spacingBetweenButtons = 260.F;

    const auto& config = ConfigManager::getInstance();

    const auto posX = static_cast< float >( config.getScreenWidth() ) * 0.83F;
    const auto startY = static_cast< float >( config.getScreenHeight() ) * 0.33F;

    float spacing = spacingBetweenButtons * config.getScale();

    const auto titleY = static_cast< float >( config.getScreenHeight() ) * 0.15F;

    const auto titlePosX = static_cast< float >( config.getScreenWidth() ) * 0.50F;
    m_settingsElements.emplace_back( Title( "Settings", sf::Vector2f{ titlePosX, titleY }, 350 * config.getScale() ) );

    m_settingsElements.emplace_back( Button( "Setting 1", sf::Vector2f{ posX, startY }, [ this ]() {} ) );

    m_settingsElements.emplace_back(
        Button( "Setting 2", sf::Vector2f{ posX, startY + spacing }, [ this ]() {} ) );

    m_settingsElements.emplace_back( Button( "Setting 3", sf::Vector2f{ posX, startY + spacing * 2 }, [ this ]() {} ) );

    m_settingsElements.emplace_back( Button( "Setting 4", sf::Vector2f{ posX, startY + spacing * 3 }, [ this ]() {} ) );

    m_settingsElements.emplace_back(
        Button( "Main Menu", sf::Vector2f{ posX, startY + spacing * 4 }, [ this ]() { m_menuState = MenuState::MainMenu; } ) );
}