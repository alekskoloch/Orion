#pragma once

#include "../pch.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../systems/SkillSystem.h"

struct SkillSchema
{
    sf::Vector2f position;
    std::string name;
    std::string description;
    std::string textureName;
    std::string hoverTextureName;
    std::string activeTextureName;
    std::function<void(entt::registry& registry)> callback;
    std::vector<SkillSchema> skillsToUnlock;
};

static SkillSchema SlaughterSurge
{
    sf::Vector2f(1520.f, 280.f),
    "Slaughter Surge",
    "Increases the damage inflicted by 5%",
    "skill",
    "skillHover",
    "skillActive",
    [](entt::registry& registry)
    {
        SkillSystem::addDamageMultiplier(registry, 0.05f);
    }
};

static SkillSchema LethalPrecision
{
    sf::Vector2f(1920.f, 680.f),
    "Lethal Precision",
    "All single shot weapons deal damage increased by 5%",
    "skill",
    "skillHover",
    "skillActive",
    [](entt::registry& registry)
    {
        SkillSystem::addSingleShotWeaponDamageMultiplier(registry, 0.05f);
    },
    {SlaughterSurge}
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
        SkillSystem::addWeaponDamageMultiplier(registry, 0.05f);
    },
    {LethalPrecision}
};