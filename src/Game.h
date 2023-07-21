#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class Game
{
private:
    sf::RenderWindow window;

    void processEvents();
public:
    Game();
    void run();
};
