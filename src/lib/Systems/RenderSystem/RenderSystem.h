#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "renderable.h"
#include "position.h"

class RenderSystem
{
public:
    static void renderEntities(sf::RenderWindow& window, entt::registry& registry);
};