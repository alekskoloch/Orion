#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>

#include "../../schema/ShieldSchema.h"

enum class SpecialShotType
{
    FullCircleShoot,
    TripleSalvo,
    None
};

enum class WeaponType
{
    SingleShot,
    TrippleShot,
    Shuriken,
    DoubleShuriken,
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

struct Experience
{
    unsigned int level = 1;
    unsigned int experience = 0;
    unsigned int experienceToNextLevel = 100;
    unsigned int skillPoints = 0;
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
    bool canGetShield = true;

    bool readyForMovementBoost = false;
    bool movementBoostActive = false;
};

struct MoneyInventory
{
    unsigned int money = 0;
};

struct MovementBoost
{
    float boostValue;
    float boostDecelerationValue;
};

struct Name
{
    std::string name;
};

struct PointOfInterest
{
    std::string id;
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
    float tripleShotWeaponDamageMultiplier = 1.f;

    float weaponEnergyCostMultiplier = 1.f;
    float singleShotWeaponEnergyCostMultiplier = 1.f;
    float tripleShotWeaponEnergyCostMultiplier = 1.f;

    float energyRegenerationMultiplier = 1.f;

    float shieldTimeDurationMultiplier = 1.f;

    bool singleShotWeaponSpecialShot = false;
    bool tripleShotWeaponSpecialShot = false;
    bool allWeaponsSpecialShot = false;

    float singleSpecialShotWeaponEnergyCostMultiplier = 1.f;
    float tripleSpecialShotWeaponEnergyCostMultiplier = 1.f;
    bool singleSpecialShotWeaponShieldChance = false;
    bool tripleSpecialShotWeaponShieldChance = false;
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
    std::string name;
    WeaponType type;

    float damage;
    float bulletSpeed;
    float shotCost;
    float specialShotCost;
    float shotCooldown;
    float specialShotCooldown;
    std::function<void(entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)> shot = [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity) {};
    std::function<void(entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)> specialShot = [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity) {};

//TODO: This should be handled in a different way
    bool autofire = false;

    unsigned bulletsInSalvo = 0;
    float queueCooldown = 0.0f;
    unsigned bulletsInQueue = 0;
    float queueCooldownTime = 0.f;    
    float currentCooldownTime = 0.0f;
    bool shootLastFrame = false;

    void SetCooldown() { currentCooldownTime = shotCooldown; }
};

struct WaypointMovement
{
    std::vector<sf::Vector2f> waypoints;
    unsigned int currentWaypointIndex;

    WaypointMovement(std::vector<sf::Vector2f> waypoints, unsigned int currentWaypointIndex = 0)
        : waypoints(waypoints), currentWaypointIndex(currentWaypointIndex) {}
};

struct Info
{
    std::string info;
    sf::Vector2f position;
    sf::Color color;
    sf::Text text;
    float displayTime = 1.f;
};