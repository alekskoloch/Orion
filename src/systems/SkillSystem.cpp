#include "SkillSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void SkillSystem::modifySkill(entt::registry& registry, SkillType skillType, float value)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());

    switch (skillType)
    {
    case SkillType::DamageMultiplierValue:
        playerSkillsComponent.damageMultiplier += value;
        break;
    case SkillType::WeaponDamageMultiplierValue:
        playerSkillsComponent.weaponDamageMultiplier += value;
        break;
    case SkillType::SingleShotWeaponDamageMultiplierValue:
        playerSkillsComponent.singleShotWeaponDamageMultiplier += value;
        break;
    case SkillType::TripleShotWeaponDamageMultiplierValue:
        playerSkillsComponent.tripleShotWeaponDamageMultiplier += value;
        break;
    case SkillType::WeaponEnergyCostMultiplierValue:                    
        playerSkillsComponent.weaponEnergyCostMultiplier += value;
        break;
    case SkillType::SingleShotWeaponEnergyCostMultiplierValue:
        playerSkillsComponent.singleShotWeaponEnergyCostMultiplier += value;
        break;
    case SkillType::TripleShotWeaponEnergyCostMultiplierValue:
        playerSkillsComponent.tripleShotWeaponEnergyCostMultiplier += value;
        break;
    case SkillType::SingleSpecialShotWeaponEnergyCostMultiplierValue:
        playerSkillsComponent.singleSpecialShotWeaponEnergyCostMultiplier += value;
        break;
    case SkillType::TripleSpecialShotWeaponEnergyCostMultiplierValue:
        playerSkillsComponent.tripleSpecialShotWeaponEnergyCostMultiplier += value;
        break;
    case SkillType::EnergyRegenerationMultiplierValue:
        playerSkillsComponent.energyRegenerationMultiplier += value;
        break;
    case SkillType::ShieldTimeDurationMultiplierValue:
        playerSkillsComponent.shieldTimeDurationMultiplier += value;
        break;
    case SkillType::SingleShotWeaponSpecialShot:
        playerSkillsComponent.singleShotWeaponSpecialShot = value == 1;
        break;
    case SkillType::TripleShotWeaponSpecialShot:
        playerSkillsComponent.tripleShotWeaponSpecialShot = value == 1;
        break;
    case SkillType::AllWeaponsSpecialShot:
        playerSkillsComponent.allWeaponsSpecialShot = value == 1;
        break;
    case SkillType::ShieldChanceForSingleSpecialShot:
        playerSkillsComponent.singleSpecialShotWeaponShieldChance = value == 1;
        break;
    case SkillType::ShieldChanceForTripleSpecialShot:
        playerSkillsComponent.tripleSpecialShotWeaponShieldChance = value == 1;
        break;
    default:
        throw std::runtime_error("Skill type not implemented");
        break;
    }
}

bool SkillSystem::isSkillEnabled(entt::registry& registry, SkillType skillType)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());

    switch (skillType)
    {
    case SkillType::SingleShotWeaponSpecialShot:
        return playerSkillsComponent.singleShotWeaponSpecialShot;
    case SkillType::TripleShotWeaponSpecialShot:
        return playerSkillsComponent.tripleShotWeaponSpecialShot;
    case SkillType::AllWeaponsSpecialShot:
        return playerSkillsComponent.allWeaponsSpecialShot;
    case SkillType::ShieldChanceForSingleSpecialShot:
        return playerSkillsComponent.singleSpecialShotWeaponShieldChance;
    case SkillType::ShieldChanceForTripleSpecialShot:
        return playerSkillsComponent.tripleSpecialShotWeaponShieldChance;
    default:
        throw std::runtime_error("Skill type not implemented");
        break;
    }
}

float SkillSystem::getValue(entt::registry& registry, SkillType skillType)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());

    switch (skillType)
    {
    case SkillType::DamageMultiplierValue:
        return playerSkillsComponent.damageMultiplier;
    case SkillType::WeaponDamageMultiplierValue:
        return playerSkillsComponent.weaponDamageMultiplier;
    case SkillType::SingleShotWeaponDamageMultiplierValue:
        return playerSkillsComponent.singleShotWeaponDamageMultiplier;
    case SkillType::TripleShotWeaponDamageMultiplierValue:
        return playerSkillsComponent.tripleShotWeaponDamageMultiplier;
    case SkillType::WeaponEnergyCostMultiplierValue:
        return playerSkillsComponent.weaponEnergyCostMultiplier;
    case SkillType::SingleShotWeaponEnergyCostMultiplierValue:
        return playerSkillsComponent.singleShotWeaponEnergyCostMultiplier;
    case SkillType::TripleShotWeaponEnergyCostMultiplierValue:
        return playerSkillsComponent.tripleShotWeaponEnergyCostMultiplier;
    case SkillType::SingleSpecialShotWeaponEnergyCostMultiplierValue:
        return playerSkillsComponent.singleSpecialShotWeaponEnergyCostMultiplier;
    case SkillType::TripleSpecialShotWeaponEnergyCostMultiplierValue:
        return playerSkillsComponent.tripleSpecialShotWeaponEnergyCostMultiplier;
    case SkillType::EnergyRegenerationMultiplierValue:
        return playerSkillsComponent.energyRegenerationMultiplier;
    case SkillType::ShieldTimeDurationMultiplierValue:
        return playerSkillsComponent.shieldTimeDurationMultiplier;
    default:
        throw std::runtime_error("Skill type not implemented");
        break;
    }
}