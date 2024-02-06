#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "FontManager.h"

#include "info.h"

class InfoSystem
{
public:
    static void update(entt::registry& registry, sf::Time deltaTime);
    static void draw(entt::registry& registry, sf::RenderWindow& window);
};