#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "SceneManager.h"

#include "GUIElement.hpp"

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

    std::vector<std::unique_ptr<GUIElement>> elements;

    void initialize();
    unsigned int SCREEN_WIDTH;
    unsigned int SCREEN_HEIGHT;

    unsigned int BUTTON_WIDTH;
    unsigned int BUTTON_HEIGHT;

    unsigned int TITLE_FONT_SIZE;
    unsigned int BUTTONS_FONT_SIZE;

    float MARGIN;
    float OUTLINE_THICKNESS;
};