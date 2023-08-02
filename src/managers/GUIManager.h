#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class GUIManager
{
public:
    GUIManager(sf::RenderWindow& window);

    void update();
    void draw();
private:
    sf::RenderWindow& window;

    void initializeQuickMenu();
    std::vector<sf::Sprite> quickMenuTiles;
};