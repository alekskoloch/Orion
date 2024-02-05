#include "Game.h"

#include "managers/SceneManager.h"

#include "systems/PlayerInitializationSystem.h"
#include "systems/EnemyInitializationSystem.h"

#include "components/components.h"
#include "components/tagComponents.h"

Game::Game() : window(sf::VideoMode(3840u, 2160u), "Orion"), systemManager(this->window, this->registry, this->event), guiManager(this->window, this->registry, this->event, this->systemManager.getQuests())    
{
    //TODO: Frame rate should be configurable
    window.setFramerateLimit(144);
}

void Game::run()
{
    while (window.isOpen())
    {
        this->processEvents();
        this->update(this->clock.restart());
        this->render();
    }
}

void Game::processEvents()
{
    this->event = sf::Event();

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab && !this->guiManager.pause())
        {
            this->systemManager.enableSlowMotion();
            this->guiManager.toggleQuickMenu(true);
        }
        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Tab)
        {
            this->systemManager.disableSlowMotion();
            this->guiManager.toggleQuickMenu(false);
        }

        //TODO: Temporary solution for testing
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            SceneManager::getInstance().setCurrentScene(Scene::Game);
        }

        //when ` key is pressed, toggle debug mode
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tilde)
        {
            this->systemManager.debugMode = !this->systemManager.debugMode;
        }
    }

    this->guiManager.processInput();
    
    if (!this->guiManager.pause())
        this->systemManager.executeEventSystems();
}

void Game::update(sf::Time deltaTime)
{
    this->guiManager.update(deltaTime);
    if (!this->guiManager.pause())
        this->systemManager.executeUpdateSystems(deltaTime);
}

void Game::render()
{
    this->window.clear();

    this->systemManager.executeRenderSystems();
    this->guiManager.draw();

    this->window.display();
}