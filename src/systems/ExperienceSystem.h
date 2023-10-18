#pragma once

#include "../pch.h"

class ExperienceSystem
{
public:
    static void addExp(entt::registry& registry, unsigned int exp);

    static void addSkillPoint(entt::registry& registry, unsigned int skillPoints);
    unsigned int getSkillPoints(entt::registry& registry);
};