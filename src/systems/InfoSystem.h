#pragma once

#include "../pch.h"

class InfoSystem
{
public:
    static void update(entt::registry& registry, sf::Time deltaTime);
    static void draw(entt::registry& registry, sf::RenderWindow& window);
};