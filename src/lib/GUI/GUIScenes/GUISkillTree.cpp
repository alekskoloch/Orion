#include "pch.h"
#include "GUISkillTree.h"

GUISkillTree::GUISkillTree(entt::registry& registry, sf::RenderWindow& window) : registry(registry), window(window)
{
    this->view = this->window.getDefaultView();
    this->view.zoom(1 / ConfigManager::getInstance().getScale());
}

void GUISkillTree::update( sf::Time& deltaTime, const Mouse::MouseState& mouseState )
{
    SkillManager::getInstance( this->registry ).update( deltaTime, mouseState );

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::W ) )
        view.move( sf::Vector2f{ 0, -moveSpeed * deltaTime.asSeconds() } );
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::A ) )
        view.move( sf::Vector2f{ -moveSpeed * deltaTime.asSeconds(), 0 } );
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::S ) )
        view.move( sf::Vector2f{ 0, moveSpeed * deltaTime.asSeconds() } );
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::D ) )
        view.move( sf::Vector2f{ moveSpeed * deltaTime.asSeconds(), 0 } );

    window.setView( view );

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) )
    {
        SceneManager::getInstance().setCurrentScene( Scene::Game );
    }
}

void GUISkillTree::draw( Window& window )
{
    SkillManager::getInstance( this->registry ).draw( window );
}