#include "Game.h"

#include "systems/PlayerInitializationSystem.h"
#include "systems/EnemyInitializationSystem.h"

#include "components/components.h"
#include "components/tagComponents.h"

Game::Game() : window(sf::VideoMode(1920u, 1080u), "Orion"), systemManager(this->window, this->registry)
{
    //TODO: Frame rate should be configurable
    window.setFramerateLimit(144);
    this->systemManager.executeInitializationSystems();
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
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    this->systemManager.executeEventSystems();
}

void Game::update(sf::Time deltaTime)
{
    this->systemManager.executeUpdateSystems(deltaTime);
}

void Game::render()
{
    this->window.clear();

    this->systemManager.executeRenderSystems();

    this->window.display();
}