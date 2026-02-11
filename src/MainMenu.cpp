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
    const auto& config = ConfigManager::getInstance();

    const auto centerX = static_cast< float >( config.getScreenWidth() ) / 2.F;
    const auto startY = static_cast< float >( config.getScreenHeight() ) * 0.4F;
    float spacing = 100.F * config.getScale();

    m_elements.emplace_back( Button( "GUI/button", sf::Vector2f{ centerX, startY },
                                     [ this ]() { *m_gameState = GameState::Game; } ) );

    m_elements.emplace_back( Button( "GUI/button", sf::Vector2f{ centerX, startY + spacing },
                                     [ this ]() { *m_gameState = GameState::Quit; } ) );
}