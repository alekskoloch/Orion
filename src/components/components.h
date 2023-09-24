#pragma once

#include "../pch.h"

#include "../managers/TextureManager.h"

#include "../schema/ShieldSchema.h"

enum class SpecialShotType
{
    FullCircleShoot,
    None
};

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

struct Cooldown
{
    std::unordered_map<std::string, float> cooldowns;
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

    bool regeneration = true;
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

struct StoneInventory
{
    unsigned int greenStones = 0;
    unsigned int orangeStones = 0;
};

struct Health
{
    float maxHealthValue;
    float currentHealthValue;
};

struct Input
{
    bool up;
    bool down;
    bool left;
    bool right;

    bool shoot;
    bool specialShot;

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

    void Set(ShieldSchema shield)
    {
        durability = shield.durability;
        energyCost = shield.energyCost;
        duration = shield.duration;
        loadTime = shield.loadTime;

        energyUsed = 0.f;

        TextureManager::getInstance().loadTexture(shield.shieldTextureName, ASSETS_PATH + shield.shieldTextureName + ".png");
        shieldTextureName = shield.shieldTextureName;
        TextureManager::getInstance().loadTexture(shield.shieldIconTextureName, ASSETS_PATH + shield.shieldIconTextureName + ".png");
        shieldIconTextureName = shield.shieldIconTextureName;
    }
};

struct Skills
{
    float damageMultiplier = 1.f;
    float weaponDamageMultiplier = 1.f;
    float singleShotWeaponDamageMultiplier = 1.f;

    float weaponEnergyCostMultiplier = 1.f;
    float singleShotWeaponEnergyCostMultiplier = 1.f;

    float singleSpecialShotWeaponEnergyCostMultiplier = 1.f;

    float energyRegenerationMultiplier = 1.f;

    float shieldTimeDurationMultiplier = 1.f;

    bool singleShotWeaponSpecialShot = false;
    bool tripleShotWeaponSpecialShot = false;
    bool allWeaponsSpecialShot = false;
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
    float damage;
    float cooldownTime;
    float bulletSpeed;
    std::string bulletTextureName;
    std::string weaponIconTextureName;
    float energyCost;

    SpecialShotType specialShotType;
    float energyCostForSpecialShot;
    
    bool autofire = false;
    float currentCooldownTime = 0.0f;
    bool shootLastFrame = false;

    void SetCooldown() { currentCooldownTime = cooldownTime; }
};

struct WaypointMovement
{
    std::vector<sf::Vector2f> waypoints;
    unsigned int currentWaypointIndex;

    WaypointMovement(std::vector<sf::Vector2f> waypoints, unsigned int currentWaypointIndex = 0)
        : waypoints(waypoints), currentWaypointIndex(currentWaypointIndex) {}
};
