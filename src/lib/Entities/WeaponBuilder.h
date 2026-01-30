#pragma once

#include "weapon.h"

#include "InputData.hpp"

using ShotFunction = std::function< void( entt::registry& registry, const Mouse::MouseState& mouseState, entt::entity& entity ) >;

class WeaponBuilder
{
public:
    WeaponBuilder();

    auto addName( std::string name ) -> WeaponBuilder&;
    auto addType( WeaponType type ) -> WeaponBuilder&;
    auto addDamage( float damage ) -> WeaponBuilder&;
    auto addBulletSpeed( float bulletSpeed ) -> WeaponBuilder&;
    auto addShotCost( float shotCost ) -> WeaponBuilder&;
    auto addSpecialShotCost( float specialShotCost ) -> WeaponBuilder&;
    auto addShotCooldown( float shotCooldown ) -> WeaponBuilder&;
    auto addSpecialShotCooldown( float specialShotCooldown ) -> WeaponBuilder&;
    auto addShotFunction( ShotFunction shotFunction ) -> WeaponBuilder&;
    auto addSpecialShotFunction( ShotFunction specialShotFunction ) -> WeaponBuilder&;

    auto build() -> Weapon;

private:
    Weapon weapon;
};