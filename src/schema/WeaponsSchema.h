#pragma once

#include "../components/components.h"

struct WeaponSchema
{
    WeaponType weaponType;
    float damage;
    float cooldownTime;
    float bulletSpeed;
    std::string bulletTextureName;
    std::string weaponIconTextureName;
    SpecialShotType specialShotType = SpecialShotType::None;
    float energyCost = 0.f;
    float energyCostForSpecialShot = 0.f;
    float specialShotCooldownTime = 0.f;
};

static WeaponSchema redWeapon =
{
    WeaponType::SingleShot,
    1.f,
    0.2f,
    1300.f,
    "red_bullet",
    "red_weapon",
    SpecialShotType::FullCircleShoot,
    10.f,
    100.f,
    5.f
};

static WeaponSchema blueWeapon =
{
    WeaponType::TrippleShot,
    1.f,
    0.4f,
    2000.f,
    "blue_bullet",
    "blue_weapon",
    SpecialShotType::FullCircleShoot,
    35.f,
    100.f,
    5.f
};


static WeaponSchema basicEnemyWeapon =
{
    WeaponType::SingleShot,
    1.f,
    2.5f,
    800.f,
    "enemy_bullet",
    "empty"
};

static WeaponSchema basicEnemyWeapon2 =
{
    WeaponType::TrippleShot,
    1.f,
    4.0f,
    500.f,
    "enemy_bullet2",
    "empty"
};

static WeaponSchema basicEnemyWeapon3 =
{
    WeaponType::SingleShot,
    1.f,
    5.0f,
    2000.f,
    "enemy_bullet3",
    "empty"
};

static WeaponSchema basicEnemyWeapon4 =
{
    WeaponType::TrippleShot,
    1.f,
    2.0f,
    800.f,
    "enemy_bullet4",
    "empty"
};

static WeaponSchema Shuriken =
{
    WeaponType::Shuriken,
    1.f,
    2.0f,
    1000.f,
    "enemy_red_bullet",
    "empty"
};