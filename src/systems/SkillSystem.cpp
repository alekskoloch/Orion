#include "SkillSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void SkillSystem::addDamageMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.damageMultiplier += multiplier;
}

void SkillSystem::addWeaponDamageMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.weaponDamageMultiplier += multiplier;
}

void SkillSystem::addSingleShotWeaponDamageMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.singleShotWeaponDamageMultiplier += multiplier;
}

float SkillSystem::getWeaponDamage(entt::registry& registry, entt::entity entity)
{
    auto& skillsComponent = registry.get<Skills>(entity);
    auto& weaponComponent = registry.get<Weapon>(entity);

    float weaponDamage = weaponComponent.damage;
    float damageMultiplier = skillsComponent.weaponDamageMultiplier;
    
    if (weaponComponent.weaponType == WeaponType::SingleShot)
        damageMultiplier += skillsComponent.singleShotWeaponDamageMultiplier - 1;

    damageMultiplier += skillsComponent.damageMultiplier - 1;

    return weaponDamage * damageMultiplier;
}

void SkillSystem::addWeaponEnergyCostMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.weaponEnergyCostMultiplier += multiplier;
}

void SkillSystem::addSingleShotWeaponEnergyCostMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.singleShotWeaponEnergyCostMultiplier += multiplier;
}

void SkillSystem::addSingleShotWeaponSpecialShotEnergyCostMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.singleSpecialShotWeaponEnergyCostMultiplier += multiplier;
}

void SkillSystem::addTripleShotWeaponSpecialShotEnergyCostMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.tripleSpecialShotWeaponEnergyCostMultiplier += multiplier;
}

float SkillSystem::getWeaponEnergyCost(entt::registry& registry, entt::entity entity)
{
    auto& skillsComponent = registry.get<Skills>(entity);
    auto& weaponComponent = registry.get<Weapon>(entity);

    float energyCost = weaponComponent.energyCost;
    float energyCostMultiplier = 1.f;

    if (weaponComponent.weaponType == WeaponType::SingleShot)
        energyCostMultiplier += skillsComponent.singleShotWeaponEnergyCostMultiplier - 1;

    return energyCost * energyCostMultiplier;
}

float SkillSystem::getWeaponSpecialShotEnergyCost(entt::registry& registry, entt::entity entity)
{
    auto& skillsComponent = registry.get<Skills>(entity);
    auto& weaponComponent = registry.get<Weapon>(entity);

    float energyCost = weaponComponent.energyCostForSpecialShot;
    float energyCostMultiplier = 1.f;

    if (weaponComponent.weaponType == WeaponType::SingleShot)
        energyCostMultiplier += skillsComponent.singleSpecialShotWeaponEnergyCostMultiplier - 1;

    return energyCost * energyCostMultiplier;
}

void SkillSystem::addEnergyRegenerationMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.energyRegenerationMultiplier += multiplier;
}

float SkillSystem::getEnergyRegeneration(entt::registry& registry, entt::entity entity)
{
    auto& skillsComponent = registry.get<Skills>(entity);
    auto& energyComponent = registry.get<Energy>(entity);

    float energyRegeneration = energyComponent.energyRegenerationRate;
    float energyRegenerationMultiplier = skillsComponent.energyRegenerationMultiplier;

    return energyRegeneration * energyRegenerationMultiplier;
}

void SkillSystem::addShieldTimeDurationMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.shieldTimeDurationMultiplier += multiplier;
}

float SkillSystem::getShieldTimeDurationMultiplier(entt::registry& registry)
{
    auto playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    return playerSkillsComponent.shieldTimeDurationMultiplier;
}

void SkillSystem::enableSingleShotWeaponSpecialShot(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.singleShotWeaponSpecialShot = true;
}

void SkillSystem::enableTripleShotWeaponSpecialShot(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.tripleShotWeaponSpecialShot = true;
}

void SkillSystem::enableAllWeaponsSpecialShot(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.allWeaponsSpecialShot = true;
}

bool SkillSystem::isSingleShotWeaponSpecialShotEnabled(entt::registry& registry)
{
    auto playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    return playerSkillsComponent.singleShotWeaponSpecialShot;
}

bool SkillSystem::isTripleShotWeaponSpecialShotEnabled(entt::registry& registry)
{
    auto playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    return playerSkillsComponent.tripleShotWeaponSpecialShot;
}

bool SkillSystem::isAllWeaponsSpecialShotEnabled(entt::registry& registry)
{
    auto playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    return playerSkillsComponent.allWeaponsSpecialShot;
}

void SkillSystem::enableShieldChanceForSingleSpecialShot(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.singleSpecialShotWeaponShieldChance = true;
}

void SkillSystem::enableShieldChanceForTripleSpecialShot(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.tripleSpecialShotWeaponShieldChance = true;
}

bool SkillSystem::getShieldChance(entt::registry& registry, entt::entity entity)
{
    auto& skillsComponent = registry.get<Skills>(entity);
    auto& weaponComponent = registry.get<Weapon>(entity);

    if (weaponComponent.weaponType == WeaponType::SingleShot)
        return skillsComponent.singleSpecialShotWeaponShieldChance;
    else
        return false;
}