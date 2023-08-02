#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class GUIManager
{
public:
    GUIManager(sf::RenderWindow& window);

    void update();
    void draw();

    void toggleQuickMenu(bool value);
private:
    sf::RenderWindow& window;

    bool quickMenuActive = false;
    void initializeQuickMenu();
    std::vector<sf::Sprite> quickMenuTiles;
};