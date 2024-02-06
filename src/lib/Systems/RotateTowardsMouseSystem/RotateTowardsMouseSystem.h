#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "CameraSystem.h"

#include "MathOperations.h"

#include "rotationTowardsMouse.h"
#include "position.h"
#include "renderable.h"

class RotateTowardsMouseSystem
{
public:
    static void rotateTowardsMouse(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window);
};