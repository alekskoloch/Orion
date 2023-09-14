#pragma once

#include "../pch.h"

class SkillSystem
{
    static void updateWeaponDamageMultiplier(entt::registry& registry, float multiplier);
    static float getWeaponDamage(entt::registry& registry, entt::entity entity); 
};