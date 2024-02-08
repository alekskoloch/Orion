#include "pch.h"
#include "WeaponBuilder.h"

WeaponBuilder::WeaponBuilder()
{
    
}

WeaponBuilder& WeaponBuilder::addName(std::string name)
{
    weapon.name = name;

    return *this;
}

WeaponBuilder& WeaponBuilder::addType(WeaponType type)
{
    weapon.type = type;

    return *this;
}

WeaponBuilder& WeaponBuilder::addDamage(float damage)
{
    weapon.damage = damage;

    return *this;
}

WeaponBuilder& WeaponBuilder::addBulletSpeed(float bulletSpeed)
{
    weapon.bulletSpeed = bulletSpeed;

    return *this;
}

WeaponBuilder& WeaponBuilder::addShotCost(float shotCost)
{
    weapon.shotCost = shotCost;

    return *this;
}

WeaponBuilder& WeaponBuilder::addSpecialShotCost(float specialShotCost)
{
    weapon.specialShotCost = specialShotCost;

    return *this;
}

WeaponBuilder& WeaponBuilder::addShotCooldown(float shotCooldown)
{
    weapon.shotCooldown = shotCooldown;

    return *this;
}

WeaponBuilder& WeaponBuilder::addSpecialShotCooldown(float specialShotCooldown)
{
    weapon.specialShotCooldown = specialShotCooldown;

    return *this;
}

WeaponBuilder& WeaponBuilder::addShotFunction(ShotFunction shotFunction)
{
    weapon.shot = shotFunction;

    return *this;
}

WeaponBuilder& WeaponBuilder::addSpecialShotFunction(ShotFunction specialShotFunction)
{
    weapon.specialShot = specialShotFunction;

    return *this;
}

Weapon WeaponBuilder::build()
{
    return weapon;
}