#pragma once

#include "../pch.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

struct SkillSchema
{
    sf::Vector2f position;
    std::string name;
    std::string description;
    std::string textureName;
    std::string hoverTextureName;
    std::string activeTextureName;
    std::function<void(entt::registry& registry)> callback;
};

static SkillSchema OrionProtocol
{
    sf::Vector2f(1920.f, 1080.f),
    "Orion protocol",
    "All bullets deal damage increased by 5%",
    "skill",
    "skillHover",
    "skillActive",
    [](entt::registry& registry)
    {
        auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
        playerSkillsComponent.weaponDamageMultiplier += 0.05f;
    }
};