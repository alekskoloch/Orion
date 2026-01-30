#pragma once

#include <functional>
#include <string>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "InputData.hpp"

enum class WeaponType : std::uint8_t
{
    SingleShot,
    TrippleShot,
    Shuriken,
    DoubleShuriken,
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
    std::function< void( entt::registry& registry, const Mouse::MouseState& mouseState, entt::entity& entity ) > shot =
        []( entt::registry& registry, const Mouse::MouseState& mouseState, entt::entity& entity ) {};
    std::function< void( entt::registry& registry, const Mouse::MouseState& mouseState, entt::entity& entity ) > specialShot =
        []( entt::registry& registry, const Mouse::MouseState& mouseState, entt::entity& entity ) {};

    // TODO: This should be handled in a different way
    bool autofire = false;

    unsigned bulletsInSalvo = 0;
    float queueCooldown = 0.0f;
    unsigned bulletsInQueue = 0;
    float queueCooldownTime = 0.f;    
    float currentCooldownTime = 0.0f;
    bool shootLastFrame = false;

    void SetCooldown() { currentCooldownTime = shotCooldown; }
};