#pragma once

#include <entt/entt.hpp>

class ExperienceSystem
{
public:
    static void addExp(entt::registry& registry, unsigned int exp);

    static void addSkillPoint(entt::registry& registry, unsigned int skillPoints = 1);
    static void removeSkillPoint(entt::registry& registry, unsigned int skillPoints = 1);
    static unsigned int getSkillPoints(entt::registry& registry);
};