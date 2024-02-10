#pragma once

#include <SFML/Graphics.hpp>

struct Particle
{
    sf::CircleShape particle;
    sf::Vector2f velocity;
    float lifetime;
};