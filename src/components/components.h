#pragma once

#include <vector>

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

struct Weapon
{
    float cooldownTime;
    bool autofire = false;
    float speed;
    std::string textureName;
    float currentCooldownTime = 0.0f;
    bool shootLastFrame = false;
};

struct WaypointMovement
{
    std::vector<sf::Vector2f> waypoints;
    unsigned int currentWaypointIndex;

    WaypointMovement(std::vector<sf::Vector2f> waypoints, unsigned int currentWaypointIndex = 0)
        : waypoints(waypoints), currentWaypointIndex(currentWaypointIndex) {}
};
