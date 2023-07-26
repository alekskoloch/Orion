#pragma once

#include <SFML/Graphics.hpp>

struct Input
{
    bool up;
    bool down;
    bool left;
    bool right;
};

struct Position
{
    sf::Vector2f position;
};

struct Renderable
{
    sf::Sprite sprite;
};

struct Speed
{
    float speed;
};



