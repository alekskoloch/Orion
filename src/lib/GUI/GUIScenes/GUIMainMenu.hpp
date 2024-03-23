#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "SceneManager.h"

class GUIMainMenu
{
public:
    GUIMainMenu(entt::registry& registry, sf::RenderWindow& window);

    void update(sf::Time& deltaTime);
    void draw();
private:
    entt::registry& registry;
    sf::RenderWindow& window;
    sf::View view;

    sf::Font font;
};