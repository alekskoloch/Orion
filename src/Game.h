#pragma once

#include "GUIManager.h"
#include "SystemManager.h"

class Game
{
private:
    sf::RenderWindow window;
    entt::registry registry;
    sf::Clock clock;
    sf::Event event;

    SystemManager systemManager;
    GUIManager guiManager;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
public:
    Game();
    void run();
};
