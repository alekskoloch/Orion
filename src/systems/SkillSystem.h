#pragma once

#include "../pch.h"

/*! @brief Enum class for all skills in the game. */
enum class SkillType
{
    /* 0 */     None = 0,
//              SKILLS CHANGING DAMAGE MULTIPLIERS
    /* 1 */     DamageMultiplierValue = 1,
    /* 2 */     WeaponDamageMultiplierValue,
    /* 3 */     SingleShotWeaponDamageMultiplierValue,
    /* 4 */     TripleShotWeaponDamageMultiplierValue,
    /* 5 */     PlasmaShotWeaponDamageMultiplierValue,                //NOT IMPLEMENTED
    /* 6 */     LaserShotWeaponDamageMultiplierValue,                 //NOT IMPLEMENTED
    /* 7 */     SingleSpecialShotWeaponDamageMultiplierValue,         //NOT IMPLEMENTED
    /* 8 */     TripleSpecialShotWeaponDamageMultiplierValue,         //NOT IMPLEMENTED
    /* 9 */     PlasmaSpecialShotWeaponDamageMultiplierValue,         //NOT IMPLEMENTED
    /* 10 */    LaserSpecialShotWeaponDamageMultiplierValue,          //NOT IMPLEMENTED
//              SKILLS CHANGING ENERGY MULTIPLIERS
    /* 11 */    EnergyCostMultiplierValue,                            //NOT IMPLEMENTED
    /* 12 */    WeaponEnergyCostMultiplierValue,
    /* 13 */    SingleShotWeaponEnergyCostMultiplierValue,
    /* 14 */    TripleShotWeaponEnergyCostMultiplierValue,
    /* 15 */    PlasmaShotWeaponEnergyCostMultiplierValue,            //NOT IMPLEMENTED
    /* 16 */    LaserShotWeaponEnergyCostMultiplierValue,             //NOT IMPLEMENTED
    /* 17 */    SingleSpecialShotWeaponEnergyCostMultiplierValue,     
    /* 18 */    TripleSpecialShotWeaponEnergyCostMultiplierValue,     
    /* 19 */    PlasmaSpecialShotWeaponEnergyCostMultiplierValue,     //NOT IMPLEMENTED
    /* 20 */    LaserSpecialShotWeaponEnergyCostMultiplierValue,      //NOT IMPLEMENTED
    /* 21 */    EnergyRegenerationMultiplierValue,
//              SKILLS CHANGING MULTIPLIERS ON SHIELDS
    /* 22 */    ShieldTimeDurationMultiplierValue,
//              ENABLED SKILLS
    /* 100 */    SingleShotWeaponSpecialShot = 100,
    /* 101 */    TripleShotWeaponSpecialShot,
    /* 102 */    AllWeaponsSpecialShot,
    /* 103 */    ShieldChanceForSingleSpecialShot,
    /* 104 */    ShieldChanceForTripleSpecialShot,
};

/*! @brief Enum class for skill requirements. */
enum class RequirementType
{
    /* 0 */     None,
//              COMMON STONES   
    /* 1 */     OrangeStone,
    /* 2 */     GreenStone,
    /* 3 */     YellowStone,
    /* 4 */     RedStone,
    /* 5 */     BlueStone,
//              POWERFUL STONES
    /* 6 */     PowerfulOrangeStone,
    /* 7 */     PowerfulGreenStone,
    /* 8 */     PowerfulYelowStone,
    /* 9 */     PowerfulRedStone,
    /* 10 */    PowerfulBlueStone,
};

/**
 * @brief System managing player skills.
 * @details Provides methods for executing and querying specific player skills.
 */
class SkillSystem
{
public:
    
    /**
     * @brief Modifies the specified skill in the game.
     * @param Reference to the registry.
     * @param Skill type to be modified.
     * @param Value to be added to the skill. For boolean skills, 1 means true.
     */
    static void modifySkill(entt::registry& registry, SkillType skillType, float value = 0);

    /**
     * @brief Checks if the specified skill is enabled.
     * @param Reference to the registry.
     * @param Skill type to be checked. The skill must be appropriate.
     * @return True if the skill is enabled, false otherwise.
     */
    static bool isSkillEnabled(entt::registry& registry, SkillType skillType);

    /**
     * @brief Gets the value of the specified skill.
     * @param Reference to the registry.
     * @param Skill type to be checked. The skill must be appropriate.
     * @return Value of the skill.
     */
    static float getValue(entt::registry& registry, SkillType skillType);
};