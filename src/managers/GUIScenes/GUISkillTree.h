#pragma once

#include "../../pch.h"

#include "../../schema/SkillSchema.h"

#include "../SkillManager.h"

class GUISkillTree
{
public:
    GUISkillTree(entt::registry& registry, sf::RenderWindow& window);

    void update();
    void draw();
private:
    entt::registry& registry;
    sf::RenderWindow& window;
    sf::View view;
    float moveSpeed = 20.0f;

    sf::Font font;
};