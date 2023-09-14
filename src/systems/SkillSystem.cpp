#include "SkillSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void SkillSystem::updateWeaponDamageMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.weaponDamageMultiplier += multiplier;
}

float SkillSystem::getWeaponDamage(entt::registry& registry, entt::entity entity)
{
    auto& skillsComponent = registry.get<Skills>(entity);
    auto& weaponComponent = registry.get<Weapon>(entity);

    return weaponComponent.damage * skillsComponent.weaponDamageMultiplier;
}