#pragma once

#include "../pch.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../systems/SkillSystem.h"

struct SkillSchema
{
    sf::Vector2f position;
    std::string name;
    std::vector<std::string> descriptions;
    std::string textureName;
    std::string hoverTextureName;
    std::string activeTextureName;
    std::vector<std::function<void(entt::registry& registry)>> callbacks;
    std::vector<SkillSchema> skillsToUnlock = {};

    unsigned int maxLevel = 1;
    unsigned int currentLevel = 0;
};

static SkillSchema EnergyFrugality
{
    sf::Vector2f(2320, 280.f),
    "Energy Frugality",
    {
        "Single shoot weapons use 5% less energy",
        "Weapons use 5% less energy"
    },
    "skill",
    "skillHover",
    "skillActive",
    {
    [](entt::registry& registry)
    {
        SkillSystem::addSingleShotWeaponEnergyCostMultiplier(registry, -0.05f);
    },
    [](entt::registry& registry)
    {
        SkillSystem::addSingleShotWeaponEnergyCostMultiplier(registry, 0.05f);
        SkillSystem::addWeaponEnergyCostMultiplier(registry, -0.05f);
    }
    },
    {},
    2
};

static SkillSchema SlaughterSurge
{
    sf::Vector2f(1520.f, 280.f),
    "Slaughter Surge",
    {"Increases the damage inflicted by 5%"},
    "skill",
    "skillHover",
    "skillActive",
    {[](entt::registry& registry)
    {
        SkillSystem::addDamageMultiplier(registry, 0.05f);
    }}
};

static SkillSchema LethalPrecision
{
    sf::Vector2f(1920.f, 680.f),
    "Lethal Precision",
    {"All single shot weapons deal damage increased by 5%"},
    "skill",
    "skillHover",
    "skillActive",
    {[](entt::registry& registry)
    {
        SkillSystem::addSingleShotWeaponDamageMultiplier(registry, 0.05f);
    }},
    {SlaughterSurge, EnergyFrugality}
};

static SkillSchema OrionProtocol
{
    sf::Vector2f(1920.f, 1080.f),
    "Orion protocol",
    {"All bullets deal damage increased by 5%"},
    "skill",
    "skillHover",
    "skillActive",
    {[](entt::registry& registry)
    {
        SkillSystem::addWeaponDamageMultiplier(registry, 0.05f);
    }},
    {LethalPrecision}
};