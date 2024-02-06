#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "input.h"
#include "player.h"

class InputSystem
{
public:
    static void processInput(entt::registry& registry, sf::Event& event);
};