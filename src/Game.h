#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class Game
{
private:
    sf::RenderWindow window;

    void processEvents();
    void update();
    void render();
public:
    Game();
    void run();
};
