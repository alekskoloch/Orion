#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class PointSystem
{
public:
    static void addPointOfInterest(entt::registry& registry, sf::Vector2f pointPosition, std::string id = "default");
    static void removePointOfInterest(entt::registry& registry, sf::Vector2f pointPosition);
    static bool isPointOfInterestActive(entt::registry& registry, std::string id);

    static void update(entt::registry& registry, sf::Time deltaTime);
};