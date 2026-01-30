#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

namespace Mouse
{
struct MouseState;
} // namespace Mouse

class RotateTowardsMouseSystem
{
public:
    static void rotateTowardsMouse( entt::registry& registry, const Mouse::MouseState& mouseState, sf::Time deltaTime );
};