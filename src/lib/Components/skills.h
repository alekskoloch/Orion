#pragma once

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