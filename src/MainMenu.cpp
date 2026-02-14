#include "MainMenu.hpp"

#include <SFML/System/Time.hpp>

#include "InputContext.hpp"
#include "Window.hpp"

MainMenu::MainMenu( GameState* gameState ) : m_gameState( gameState ) 
{
    initButtons();
}

void MainMenu::handleEvent( const InputContext& inputContext )
{
    for ( auto& element : m_elements )
    {
        element.handleEvent( inputContext );
    }
}

void MainMenu::update( const sf::Time deltaTime )
{
    for ( auto& element : m_elements )
    {
        element.update( deltaTime );
    }
}

void MainMenu::draw( Window& window )
{
    for ( auto& element : m_elements )
    {
        element.draw( window );
    }
}

void MainMenu::initButtons()
{
    constexpr auto margin = 200.F;
    constexpr auto spacingBetweenButtons = 260.F;

    const auto& config = ConfigManager::getInstance();

    const auto posX = static_cast< float >( config.getScreenWidth() ) * 0.83F;
    const auto startY = static_cast< float >( config.getScreenHeight() ) * 0.33F;
    
    float spacing = spacingBetweenButtons * config.getScale();
    
    const auto titleY = static_cast< float >( config.getScreenHeight() ) * 0.30F;
    
    const auto titlePosX = static_cast< float >( config.getScreenWidth() ) * 0.30F;
    m_elements.emplace_back( Title( "Orion", sf::Vector2f{ titlePosX, titleY }, 550 * config.getScale() ) );


    m_elements.emplace_back( Button( "GUI/button", "Continue", sf::Vector2f{ posX, startY },
                                     [ this ]() { *m_gameState = GameState::Game; } ) );

    m_elements.emplace_back( Button( "GUI/button", "Start Game", sf::Vector2f{ posX, startY + spacing },
                                     [ this ]() { *m_gameState = GameState::Game; } ) );

    m_elements.emplace_back( Button( "GUI/button", "Load Game", sf::Vector2f{ posX, startY + spacing * 2 },
                                     [ this ]() {} ) );

    m_elements.emplace_back( Button( "GUI/button", "Settings", sf::Vector2f{ posX, startY + spacing * 3 },
                                     [ this ]() {} ) );

    m_elements.emplace_back( Button( "GUI/button", "Quit", sf::Vector2f{ posX, startY + spacing * 4 },
                                     [ this ]() { *m_gameState = GameState::Quit; } ) );
}