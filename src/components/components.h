#pragma once

#include <SFML/Graphics.hpp>

struct Position
{
    sf::Vector2f position;
};

struct Renderable
{
    sf::Sprite sprite;
};

struct Texture
{
    sf::Texture texture;
};
