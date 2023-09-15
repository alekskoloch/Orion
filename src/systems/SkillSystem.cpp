#include "SkillSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

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

    return weaponDamage * damageMultiplier;
}