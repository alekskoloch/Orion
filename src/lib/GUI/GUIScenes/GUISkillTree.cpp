#include "pch.h"
#include "GUISkillTree.h"

#include "Window.hpp"

#include "SkillManager.h"

GUISkillTree::GUISkillTree(entt::registry& registry) : registry(registry)
{
    auto width = static_cast< float >( ConfigManager::getInstance().getScreenWidth() );
    auto height = static_cast< float >( ConfigManager::getInstance().getScreenHeight() );

    constexpr auto halfDivider{ 2.0F };

    this->view.setSize( sf::Vector2f{ width, height } );
    this->view.setCenter( sf::Vector2f{ width / halfDivider, height / halfDivider } );

    this->view.zoom( 1.F / ConfigManager::getInstance().getScale() );
}

void GUISkillTree::update( sf::Time& deltaTime, const Mouse::MouseState& mouseState )
{
    SkillManager::getInstance( this->registry ).update( deltaTime, mouseState );

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::W ) )
    {
        view.move( sf::Vector2f{ 0, -moveSpeed * deltaTime.asSeconds() } );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::A ) )
    {
        view.move( sf::Vector2f{ -moveSpeed * deltaTime.asSeconds(), 0 } );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::S ) )
    {
        view.move( sf::Vector2f{ 0, moveSpeed * deltaTime.asSeconds() } );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::D ) )
    {
        view.move( sf::Vector2f{ moveSpeed * deltaTime.asSeconds(), 0 } );
    }

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) )
    {
        // TODO: escape from SkillTree
    }
}

void GUISkillTree::draw( Window& window )
{
    const sf::View previousView = window.getView();

    window.setView( this->view );

    SkillManager::getInstance( this->registry ).draw( window );

    window.setView( previousView );
}

auto GUISkillTree::getView() const -> const sf::View&
{
    return this->view;
}