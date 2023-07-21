#include "Game.h"

Game::Game() : window(sf::VideoMode(1920u, 1080u), "Orion")
{

}

void Game::run()
{
    while (window.isOpen())
    {
        
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
    this->window.display();
}