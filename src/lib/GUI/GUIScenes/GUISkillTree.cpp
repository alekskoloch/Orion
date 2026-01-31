#include "pch.h"
#include "GUISkillTree.h"

#include "Window.hpp"

GUISkillTree::GUISkillTree(entt::registry& registry, sf::RenderWindow& window) : registry(registry), window(window)
{
    this->view = this->window.getDefaultView();
    this->view.zoom(1 / ConfigManager::getInstance().getScale());
}

void GUISkillTree::update( sf::Time& deltaTime, const Mouse::MouseState& mouseState )
{
    sf::Vector2i pixelPos = sf::Vector2i(
        static_cast<int>(mouseState.screenPosition.x), 
        static_cast<int>(mouseState.screenPosition.y)
    );

    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, this->view);

    Mouse::MouseState localMouseState = mouseState;
    localMouseState.worldPosition = { .x=worldPos.x, .y=worldPos.y };

    SkillManager::getInstance( this->registry ).update( deltaTime, localMouseState );

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::W ) )
        view.move( sf::Vector2f{ 0, -moveSpeed * deltaTime.asSeconds() } );
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::A ) )
        view.move( sf::Vector2f{ -moveSpeed * deltaTime.asSeconds(), 0 } );
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::S ) )
        view.move( sf::Vector2f{ 0, moveSpeed * deltaTime.asSeconds() } );
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::D ) )
        view.move( sf::Vector2f{ moveSpeed * deltaTime.asSeconds(), 0 } );

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) )
    {
        SceneManager::getInstance().setCurrentScene( Scene::Game );
    }
}

void GUISkillTree::draw( Window& window )
{
    const sf::View previousView = window.getView();

    window.setView( this->view );

    SkillManager::getInstance( this->registry ).draw( window );

    window.setView( previousView );
}