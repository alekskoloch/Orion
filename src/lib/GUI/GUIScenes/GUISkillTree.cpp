#include "pch.h"
#include "GUISkillTree.h"

GUISkillTree::GUISkillTree(entt::registry& registry, sf::RenderWindow& window) : registry(registry), window(window)
{
    this->view = this->window.getDefaultView();
    this->view.zoom(1 / ConfigManager::getInstance().getScale());
}

void GUISkillTree::update(sf::Time& deltaTime)
{
    SkillManager::getInstance(this->window, this->registry).update(deltaTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        view.move(0, -moveSpeed * deltaTime.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        view.move(-moveSpeed * deltaTime.asSeconds(), 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        view.move(0, moveSpeed * deltaTime.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        view.move(moveSpeed * deltaTime.asSeconds(), 0);

    window.setView(view);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        SceneManager::getInstance().setCurrentScene(Scene::Game);
    }
}

void GUISkillTree::draw()
{
    SkillManager::getInstance(this->window, this->registry).draw();
}