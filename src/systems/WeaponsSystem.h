#pragma once

#include "../pch.h"

#include "../schema/WeaponsSchema.h"

class WeaponsSystem
{
public:
    static void changeWeapon(entt::registry& registry, WeaponSchema weapon);
    static void loadWeapon(entt::registry& registry, const WeaponSchema& weaponSchema, entt::entity ownerEntity);
    static void updateWeaponCooldown(entt::registry& registry, sf::Time deltaTime);
    static float getWeaponDamage(entt::registry& registry);
    static float getWeaponShotEnergyCost(entt::registry& registry);
    static float getWeaponSpecialShotEnergyCost(entt::registry& registry);
};