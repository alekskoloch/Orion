#pragma once

#include <SFML/Graphics.hpp>

struct Acceleration
{
    float accelerationValue;
    float decelerationValue;
};

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

struct RotationTowardsMouse
{
    bool enabled;
    float sensitivity;
    float minimalActivationDistance;
};

struct Speed
{
    float maxSpeedValue;
};

struct Velocity
{
    sf::Vector2f velocity;
};
