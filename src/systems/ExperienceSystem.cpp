#include "ExperienceSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void ExperienceSystem::addExp(entt::registry& registry, int exp)
{
    auto& playerExp = registry.get<Experience>(registry.view<Player>()[0]).experience;
    playerExp += exp;
}