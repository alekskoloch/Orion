#pragma once

#include "../pch.h"

class RenderSystem
{
public:
    static void renderEntities(sf::RenderWindow& window, entt::registry& registry);
};