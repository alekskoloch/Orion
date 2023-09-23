#pragma once

#include "../pch.h"

class CooldownSystem
{
public:
    static void updateCooldowns(entt::registry& registry, sf::Time deltaTime);
    static void setCooldown(entt::registry& registry, entt::entity entity, std::string cooldownName, float cooldownTime);
    static float getCooldown(entt::registry& registry, entt::entity entity, std::string cooldownName);
};