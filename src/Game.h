#pragma once

#include "pch.h"

#include "managers/GUIManager.h"
#include "managers/SystemManager.h"

class Game
{
private:
    sf::RenderWindow window;
    entt::registry registry;
    sf::Clock clock;

    SystemManager systemManager;
    GUIManager guiManager;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
public:
    Game();
    void run();
};
