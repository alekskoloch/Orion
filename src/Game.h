#pragma once

#include "Window.hpp"

#include "GameState.hpp"

#include "GUIManager.h"
#include "SystemManager.h"

class Game
{
public:
    Game();
    void run();

private:
    Window m_window;
    entt::registry registry;
    sf::Clock clock;
    sf::Event event;

    GameState m_gameState{ GameState::Menu };

    SystemManager systemManager;
    GUIManager guiManager;

    // TODO: this is only temporary cursor, create proper cursor class
    void loadCursor();
    sf::Texture cursorTexture;
    sf::Cursor cursor;

    sf::View m_gameView;

    void processEvents();
    void update( sf::Time deltaTime );
    void render();
};
