#pragma once

#include "GUIManager.h"
#include "SystemManager.h"

class Game
{
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    entt::registry registry;
    sf::Clock clock;
    sf::Event event{};

    SystemManager systemManager;
    GUIManager guiManager;

    // TODO: this is only temporary cursor, create proper cursor class
    void loadCursor();
    sf::Texture cursorTexture;
    sf::Cursor cursor;

    void processEvents();
    void update( sf::Time deltaTime );
    void render();
};
