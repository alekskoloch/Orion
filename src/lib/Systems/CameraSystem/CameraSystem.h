#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "position.h"
#include "player.h"

class CameraSystem
{
public:
    static void updateCamera( sf::View& view, entt::registry& registry, float zoomFactor );
};