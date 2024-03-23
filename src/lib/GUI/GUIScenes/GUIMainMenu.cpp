#include "pch.h"
#include "GUIMainMenu.hpp"

GUIMainMenu::GUIMainMenu(entt::registry& registry, sf::RenderWindow& window) : registry(registry), window(window)
{
    this->view = this->window.getDefaultView();
}

void GUIMainMenu::update(sf::Time& deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        SceneManager::getInstance().setCurrentScene(Scene::Game);
    }
}

void GUIMainMenu::draw()
{
    sf::RectangleShape background(sf::Vector2f(1920, 1080));
    background.setFillColor(sf::Color::Red);
    window.draw(background);
}