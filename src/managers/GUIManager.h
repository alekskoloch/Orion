#pragma once

#include <SFML/Graphics.hpp>

class GUIManager
{
public:
    GUIManager(sf::RenderWindow& window);

    void draw();
private:
    sf::RenderWindow& window;
};