#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class RenderSystem
{
public:
    static void renderEntities(sf::RenderWindow& window, entt::registry& registry);
};