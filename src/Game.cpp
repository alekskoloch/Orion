#include "pch.h"
#include "Game.h"

#include "SceneManager.h"
#include "TextureManager.h"
#include "ConfigManager.hpp"

#include "PlayerInitializationSystem.h"
#include "EnemyInitializationSystem.h"

#include "nlohmann/json.hpp"

Game::Game() : window(sf::VideoMode(ConfigManager::getInstance().getScreenWidth(), ConfigManager::getInstance().getScreenHeight()), "Orion", sf::Style::Fullscreen), systemManager(this->window, this->registry, this->event), guiManager(this->window, this->registry, this->event, this->systemManager.getQuests())    
{
    this->window.setFramerateLimit(ConfigManager::getInstance().getFrameRateLimit());

    this->cursorTexture = TextureManager::getInstance().getTexture("Cursor");

    if (!this->cursor.loadFromPixels(this->cursorTexture.copyToImage().getPixelsPtr(), this->cursorTexture.getSize(), {25, 25}))
    {
        std::cerr << "Failed to load cursor texture" << std::endl;
    }

    this->window.setMouseCursor(this->cursor);

    SceneManager::getInstance().setCurrentScene(Scene::MainMenu);
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