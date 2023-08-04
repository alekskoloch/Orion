#pragma once

#include "../components/components.h"

struct EnemySchema
{
    std::string textureName;
    sf::Vector2f position;
    float speed;
    std::vector<sf::Vector2f> waypoints;
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
};