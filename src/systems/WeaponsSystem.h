#pragma once

#include "../pch.h"

#include "../schema/WeaponsSchema.h"

class WeaponsSystem
{
public:
    static void changeWeapon(entt::registry& registry, WeaponSchema weapon);
    static void loadWeapon(entt::registry& registry, const WeaponSchema& weaponSchema, entt::entity ownerEntity);
};