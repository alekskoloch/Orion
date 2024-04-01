#include "pch.h"
#include "GUIMainMenu.hpp"

GUIMainMenu::GUIMainMenu(entt::registry& registry, sf::RenderWindow& window) : registry(registry), window(window)
{
    this->initialize();
    this->view = this->window.getDefaultView();

    elements.push_back(GUIElementFactory::createText(
        sf::Vector2f(this->SCREEN_WIDTH / 2, this->SCREEN_HEIGHT / 4),
        "Orion",
        this->TITLE_FONT_SIZE
    ));

    elements.push_back(GUIElementFactory::createButton(
        sf::Vector2f(this->SCREEN_WIDTH / 2, this->SCREEN_HEIGHT / 2 - (this->BUTTON_HEIGHT + this->MARGIN)),
        sf::Vector2f(this->BUTTON_WIDTH, this->BUTTON_HEIGHT),
        "Continue",
        this->OUTLINE_THICKNESS,
        this->BUTTONS_FONT_SIZE
    ));

    elements.back().get()->setOnActivateAction([&]()
    {
        SceneManager::getInstance().setCurrentScene(Scene::Game);
    });

    elements.push_back(GUIElementFactory::createButton(
        sf::Vector2f(this->SCREEN_WIDTH / 2, this->SCREEN_HEIGHT / 2),
        sf::Vector2f(this->BUTTON_WIDTH, this->BUTTON_HEIGHT),
        "New Game",
        this->OUTLINE_THICKNESS,
        this->BUTTONS_FONT_SIZE
    ));

    elements.back().get()->setOnActivateAction([&]()
    {
        SceneManager::getInstance().setGameStarted(false);
        SceneManager::getInstance().setCurrentScene(Scene::Game);
    });

    elements.push_back(GUIElementFactory::createButton(
        sf::Vector2f(this->SCREEN_WIDTH / 2, this->SCREEN_HEIGHT / 2 + (this->BUTTON_HEIGHT + this->MARGIN)),
        sf::Vector2f(this->BUTTON_WIDTH, this->BUTTON_HEIGHT),
        "Settings",
        this->OUTLINE_THICKNESS,
        this->BUTTONS_FONT_SIZE
    ));

    elements.back().get()->setOnActivateAction([&]()
    {
        SceneManager::getInstance().setCurrentScene(Scene::Settings);
    });

    elements.push_back(GUIElementFactory::createButton(
        sf::Vector2f(this->SCREEN_WIDTH / 2, this->SCREEN_HEIGHT / 2 + 2 * (this->BUTTON_HEIGHT + this->MARGIN)),
        sf::Vector2f(this->BUTTON_WIDTH, this->BUTTON_HEIGHT),
        "Quit",
        this->OUTLINE_THICKNESS,
        this->BUTTONS_FONT_SIZE
    ));

    elements.back().get()->setOnActivateAction([&]()
    {
        this->window.close();
    });
}

void GUIMainMenu::update(sf::Time& deltaTime)
{
    sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(this->window).x, sf::Mouse::getPosition(this->window).y);

    for (size_t i = 0; i < elements.size(); i++)
    {
        if (i == 1)
            if (!SceneManager::getInstance().isGameStarted())
                continue;

        elements[i]->update(deltaTime, mousePosition);
    }
}

void GUIMainMenu::draw()
{
    for (size_t i = 0; i < elements.size(); i++)
    {
        if (i == 1)
            if (!SceneManager::getInstance().isGameStarted())
                continue;

        this->window.draw(*elements[i]);
    }
}

void GUIMainMenu::initialize()
{
    this->SCREEN_WIDTH = this->window.getSize().x;
    this->SCREEN_HEIGHT = this->window.getSize().y;

    this->BUTTON_WIDTH = this->SCREEN_WIDTH / 6;
    this->BUTTON_HEIGHT = this->SCREEN_HEIGHT / 20;

    this->TITLE_FONT_SIZE = SCREEN_WIDTH / 10;
    this->BUTTONS_FONT_SIZE = BUTTON_HEIGHT / 4;

    this->MARGIN = this->BUTTON_HEIGHT / 3;
    this->OUTLINE_THICKNESS = this->SCREEN_HEIGHT / 200;
}