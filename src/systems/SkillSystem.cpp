#include "SkillSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void SkillSystem::updateWeaponDamageMultiplier(entt::registry& registry, float multiplier)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    playerSkillsComponent.weaponDamageMultiplier += multiplier;
}