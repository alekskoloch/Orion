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

//TODO: Make template function for loading schema
void WeaponsSystem::loadWeapon(entt::registry& registry, const WeaponSchema& weaponSchema, entt::entity ownerEntity)
{
    TextureManager::getInstance().loadTexture(weaponSchema.bulletTextureName, ASSETS_PATH + weaponSchema.bulletTextureName + ".png");
    TextureManager::getInstance().loadTexture(weaponSchema.weaponIconTextureName, ASSETS_PATH + weaponSchema.weaponIconTextureName + ".png");
    registry.emplace<Weapon>(ownerEntity, weaponSchema.weaponType, weaponSchema.damage, weaponSchema.cooldownTime, weaponSchema.bulletSpeed, weaponSchema.bulletTextureName, weaponSchema.weaponIconTextureName, weaponSchema.energyCost);
}

void WeaponsSystem::updateWeaponCooldown(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Weapon>();
    for (auto& entity : view)
    {
        auto& weapon = view.get<Weapon>(entity);
        if (weapon.currentCooldownTime > 0.f)
        {
            weapon.currentCooldownTime -= deltaTime.asSeconds();
            if (weapon.currentCooldownTime < 0.f)
                weapon.currentCooldownTime = 0.f;
        }
    }
}