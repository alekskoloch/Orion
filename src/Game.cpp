#include "Game.h"

#include "systems/PlayerInitializationSystem.h"
#include "systems/RenderSystem.h"

#include "components/components.h"
#include "components/tagComponents.h"

Game::Game() : window(sf::VideoMode(1920u, 1080u), "Orion")
{
    PlayerInitializationSystem playerInitializationSystem;
    playerInitializationSystem.initializePlayer(this->registry);
}

void Game::run()
{
    while (window.isOpen())
    {
        this->processEvents();
        this->update();
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
}

void Game::update()
{

}

void Game::render()
{
    this->window.clear();

    RenderSystem::renderEntities(this->window, this->registry);

    this->window.display();
}