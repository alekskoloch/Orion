#pragma once

#include "../components/components.h"

struct WeaponSchema
{
    float cooldownTime;
    float bulletSpeed;
    std::string bulletTextureName;
};

static WeaponSchema blueWeapon =
{
    0.4f,
    2000.f,
    "blue_bullet"
};

static WeaponSchema redWeapon =
{
    0.2f,
    1300.f,
    "red_bullet"
};