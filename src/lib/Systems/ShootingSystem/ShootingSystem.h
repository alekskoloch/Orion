#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

namespace Mouse
{
struct MouseState;
} // namespace Mouse

class ShootingSystem
{
public:
    static void shoot(entt::registry& registry, sf::Time deltaTime, const Mouse::MouseState& mouseState);

private:
    static void handleQueue(entt::registry& registry, sf::Time deltaTime, const Mouse::MouseState& mouseState);
};