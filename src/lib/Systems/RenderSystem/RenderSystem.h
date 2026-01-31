#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "renderable.h"
#include "position.h"

class Window;

class RenderSystem
{
public:
    static void renderEntities( Window& window, entt::registry& registry);
};