#pragma once

#include "../components/components.h"

struct WeaponSchema
{
    WeaponType weaponType;
    float cooldownTime;
    float bulletSpeed;
    std::string bulletTextureName;
};

static WeaponSchema redWeapon =
{
    WeaponType::SingleShot,
    0.2f,
    1300.f,
    "red_bullet"
};

static WeaponSchema blueWeapon =
{
    WeaponType::TrippleShot,
    0.4f,
    2000.f,
    "blue_bullet"
};
