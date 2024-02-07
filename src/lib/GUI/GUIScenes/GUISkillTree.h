#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "SkillManager.h"

#include "GraphicsOperations.h"
#include "Mouse.h"

class GUISkillTree
{
public:
    GUISkillTree(entt::registry& registry, sf::RenderWindow& window);

    void update(sf::Time& deltaTime);
    void draw();
private:
    entt::registry& registry;
    sf::RenderWindow& window;
    sf::View view;
    float moveSpeed = 20.0f;

    sf::Font font;
};