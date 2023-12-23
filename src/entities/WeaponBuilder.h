#pragma once

#include "../pch.h"

#include "../systems/WeaponsSystem.h"

#include "../components/components.h"

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