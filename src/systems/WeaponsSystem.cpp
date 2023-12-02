#include "WeaponsSystem.h"

#include "../managers/TextureManager.h"

#include "../systems/PlayerInitializationSystem.h"
#include "../systems/SkillSystem.h"

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
    registry.emplace<Weapon>(ownerEntity, weaponSchema.weaponType, weaponSchema.damage, weaponSchema.cooldownTime, weaponSchema.bulletSpeed, weaponSchema.bulletTextureName, weaponSchema.weaponIconTextureName, weaponSchema.energyCost, weaponSchema.specialShotType, weaponSchema.energyCostForSpecialShot, weaponSchema.specialShotCooldownTime, weaponSchema.shoot, weaponSchema.specialShoot, weaponSchema.bulletsInSalvo, weaponSchema.queueCooldown);
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

float WeaponsSystem::getWeaponDamage(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    
    auto& playerWeapon = registry.get<Weapon>(registry.view<Player>().front());

    float damage = playerWeapon.damage;
    float multiplier = 1.f;

    multiplier += playerSkillsComponent.damageMultiplier - 1.f;
    multiplier += playerSkillsComponent.weaponDamageMultiplier - 1.f;

    if (playerWeapon.weaponType == WeaponType::SingleShot)
    {
        multiplier += playerSkillsComponent.singleShotWeaponDamageMultiplier - 1.f;
    }
    else if (playerWeapon.weaponType == WeaponType::TrippleShot)
    {
        multiplier += playerSkillsComponent.tripleShotWeaponDamageMultiplier - 1.f;
    }

    return damage * multiplier;
}

float WeaponsSystem::getWeaponShotEnergyCost(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());

    auto& playerWeapon = registry.get<Weapon>(registry.view<Player>().front());

    float energyCost = playerWeapon.energyCost;
    float multiplier = 1.f;

    multiplier += playerSkillsComponent.weaponEnergyCostMultiplier - 1.f;

    if (playerWeapon.weaponType == WeaponType::SingleShot)
    {
        multiplier += playerSkillsComponent.singleShotWeaponEnergyCostMultiplier - 1.f;
    }
    else if (playerWeapon.weaponType == WeaponType::TrippleShot)
    {
        multiplier += playerSkillsComponent.tripleShotWeaponEnergyCostMultiplier - 1.f;
    }

    return energyCost * multiplier;
}

float WeaponsSystem::getWeaponSpecialShotEnergyCost(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());

    auto& playerWeapon = registry.get<Weapon>(registry.view<Player>().front());

    float energyCost = playerWeapon.energyCostForSpecialShot;
    float multiplier = 1.f;

    multiplier += playerSkillsComponent.weaponEnergyCostMultiplier - 1.f;

    if (playerWeapon.weaponType == WeaponType::SingleShot)
    {
        multiplier += playerSkillsComponent.singleSpecialShotWeaponEnergyCostMultiplier - 1.f;
    }
    else if (playerWeapon.weaponType == WeaponType::TrippleShot)
    {
        multiplier += playerSkillsComponent.tripleSpecialShotWeaponEnergyCostMultiplier - 1.f;
    }

    return energyCost * multiplier;
}