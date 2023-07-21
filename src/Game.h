#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class Game
{
private:
    sf::RenderWindow window;
public:
    Game();
    void run();
};
