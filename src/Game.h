#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class Game
{
private:
    sf::RenderWindow window;
    entt::registry registry;
    sf::Clock clock;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
public:
    Game();
    void run();
};
