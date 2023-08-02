#pragma once

#include <entt/entt.hpp>

#include "../schema/WeaponsSchema.h"

class PlayerInitializationSystem
{
public:
    static void initializePlayer(entt::registry& registry);
//TODO: Move this to a separate system
    static void changeWeapon(entt::registry& registry, const WeaponSchema& weaponSchema);

private:
//TODO: Move this to a separate system
    static void loadWeapon(entt::registry& registry, const WeaponSchema& weaponSchema, entt::entity ownerEntity);
};