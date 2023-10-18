#include "ExperienceSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void ExperienceSystem::addExp(entt::registry& registry, unsigned int exp)
{
    auto& playerLevel = registry.get<Experience>(registry.view<Player>()[0]).level;
    auto& playerExp = registry.get<Experience>(registry.view<Player>()[0]).experience;
    auto& expToNextLevel = registry.get<Experience>(registry.view<Player>()[0]).experienceToNextLevel;
    playerExp += exp;

    while (playerExp >= expToNextLevel)
    {
        playerLevel++;
        ExperienceSystem::addSkillPoint(registry);
        playerExp -= registry.get<Experience>(registry.view<Player>()[0]).experienceToNextLevel;
        expToNextLevel = (unsigned int)(expToNextLevel * 1.5f);
    }
}

void ExperienceSystem::addSkillPoint(entt::registry& registry, unsigned int skillPoints = 1)
{
    registry.get<Experience>(registry.view<Player>()[0]).skillPoints += skillPoints;
}