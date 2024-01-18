#pragma once

#include "../pch.h"

class PointSystem
{
public:
    static void addPointOfInterest(entt::registry& registry, sf::Vector2f pointPosition);
    static void removePointOfInterest(entt::registry& registry, sf::Vector2f pointPosition);

    static void update(entt::registry& registry, sf::Time deltaTime);
};