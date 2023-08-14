#pragma once

#include "../components/components.h"

struct WeaponSchema
{
    WeaponType weaponType;
    float cooldownTime;
    float bulletSpeed;
    std::string bulletTextureName;
    std::string weaponIconTextureName;
};

static WeaponSchema redWeapon =
{
    WeaponType::SingleShot,
    0.2f,
    1300.f,
    "red_bullet",
    "red_weapon"
};

static WeaponSchema blueWeapon =
{
    WeaponType::TrippleShot,
    0.4f,
    2000.f,
    "blue_bullet",
    "blue_weapon"
};


static WeaponSchema basicEnemyWeapon =
{
    WeaponType::SingleShot,
    2.5f,
    800.f,
    "enemy_bullet",
    "empty"
};

static WeaponSchema basicEnemyWeapon2 =
{
    WeaponType::TrippleShot,
    4.0f,
    500.f,
    "enemy_bullet2",
    "empty"
};

static WeaponSchema basicEnemyWeapon3 =
{
    WeaponType::SingleShot,
    5.0f,
    2000.f,
    "enemy_bullet3",
    "empty"
};