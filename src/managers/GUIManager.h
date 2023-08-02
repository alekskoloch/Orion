#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class GUIManager
{
public:
    GUIManager(sf::RenderWindow& window);

    void draw();
private:
    sf::RenderWindow& window;

    void initializeQuickMenu();
    std::vector<sf::Sprite> quickMenuTiles;

    //for test
    // sf::Sprite quickMenuTileActive;
    // sf::Sprite quickMenuTileInactive2;
};