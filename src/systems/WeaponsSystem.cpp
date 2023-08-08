#include "WeaponsSystem.h"

#include "../managers/TextureManager.h"

#include "../systems/PlayerInitializationSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void WeaponsSystem::changeWeapon(entt::registry& registry, WeaponSchema weapon)
{
    auto player = registry.view<Player>();
    for (auto& entity : player)
    {
        registry.remove<Weapon>(entity);
        WeaponsSystem::loadWeapon(registry, weapon, entity);
    }
}

void WeaponsSystem::loadWeapon(entt::registry& registry, const WeaponSchema& weaponSchema, entt::entity ownerEntity)
{
    TextureManager::getInstance().loadTexture(weaponSchema.bulletTextureName, ASSETS_PATH + weaponSchema.bulletTextureName + ".png");
    TextureManager::getInstance().loadTexture(weaponSchema.weaponIconTextureName, ASSETS_PATH + weaponSchema.weaponIconTextureName + ".png");
    registry.emplace<Weapon>(ownerEntity, weaponSchema.weaponType, weaponSchema.cooldownTime, weaponSchema.bulletSpeed, weaponSchema.bulletTextureName, weaponSchema.weaponIconTextureName);
}