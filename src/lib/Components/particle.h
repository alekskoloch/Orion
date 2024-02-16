#pragma once

#include <SFML/Graphics.hpp>

struct Particle
{
    sf::CircleShape particle;
    sf::Vector2f velocity;
    float lifetime;

    sf::Color startColor = sf::Color::White;
    sf::Color endColor = sf::Color::White;
};