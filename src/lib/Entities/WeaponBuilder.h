#pragma once

#include "weapon.h"

using ShotFunction = std::function<void(entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)>;

class WeaponBuilder
{
public:
    WeaponBuilder();

    WeaponBuilder& addName(std::string name);
    WeaponBuilder& addType(WeaponType type);
    WeaponBuilder& addDamage(float damage);
    WeaponBuilder& addBulletSpeed(float bulletSpeed);
    WeaponBuilder& addShotCost(float shotCost);
    WeaponBuilder& addSpecialShotCost(float specialShotCost);
    WeaponBuilder& addShotCooldown(float shotCooldown);
    WeaponBuilder& addSpecialShotCooldown(float specialShotCooldown);
    WeaponBuilder& addShotFunction(ShotFunction shotFunction);
    WeaponBuilder& addSpecialShotFunction(ShotFunction specialShotFunction);
    
    Weapon build();

private:
    Weapon weapon;
};