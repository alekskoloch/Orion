#pragma once

#include "../pch.h"

enum class WeaponType
{
    SingleShot,
    TrippleShot
};

struct Acceleration
{
    float accelerationValue;
    float decelerationValue;
};

struct Collision
{
    sf::FloatRect collisionBox;
};

struct Drop
{
    int dropChance;
    int dropRate;
};

struct Energy
{
    float maxEnergyValue;
    float currentEnergyValue;
    float energyRegenerationRate;
};

struct EntityState
{
    enum class State
    {
        Idle,
        Attacking
    };

    State currentState = State::Idle;

    float attackRange;
    float idleRange;
};

struct Input
{
    bool up;
    bool down;
    bool left;
    bool right;

    bool shoot;

    bool isGettingShield;
    bool getShield;
};

struct Name
{
    std::string name;
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

struct Shield
{
    float durability;
    float energyCost;
    float duration;
    float loadTime;

    std::string shieldTextureName;
    std::string shieldIconTextureName;
    
    float currentDuration;

    float energyUsed = 0.f;

    bool active = false;
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
    WeaponType weaponType;
    float cooldownTime;
    float bulletSpeed;
    std::string bulletTextureName;
    std::string weaponIconTextureName;
    float energyCost;
    
    bool autofire = false;
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
