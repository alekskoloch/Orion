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

float SkillSystem::getShieldTimeDuration(entt::registry& registry, entt::entity entity)
{
    auto& skillsComponent = registry.get<Skills>(entity);
    auto& shieldComponent = registry.get<Shield>(entity);

    float shieldTimeDuration = shieldComponent.duration;
    float shieldTimeDurationMultiplier = skillsComponent.shieldTimeDurationMultiplier;

    return shieldTimeDuration * shieldTimeDurationMultiplier;
}