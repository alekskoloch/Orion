#pragma once

#include "../pch.h"

class ShootingSystem
{
public:
    static void shoot(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window);

private:
    static void handleQueue(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window);
};