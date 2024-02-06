#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "TextureManager.h"
#include <fstream>
#include <iostream>

#include "WeaponsSchema.h"

//TODO: Resolve this dependency
#include "../Systems/SkillSystem/SkillSystem.h"

#include "../entities/WeaponBuilder.h"

#include "weapon.h"
#include "skills.h"
#include "enemy.h"

using ShotFunction = std::function<void(entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)>;

enum class Weapons
{
    None = 0,
    RedWeapon = 1,
    BlueWeapon,
    QuadWeapon,
    GreyPebble = 100,
    SmallFireball,
    DoubleFireball,
    GreenGunk,
    Brain
};

enum class ShotType
{
    /* 0 */     None = 0,
//              BASIC TYPES OF SHOTS
    /* 1 */     SingleShot = 1,
    /* 2 */     TripleShot,
    /* 3 */     Shuriken,
    /* 4 */     DoubleShuriken,
    /* 5 */     Nail,
    /* 6 */     QuadShot,
//              SPECIAL TYPES OF SHOTS
    /* 100 */   FullCircle = 100,
    /* 101 */   TripleSalvo,
    /* 102 */   SpinningShot
};

class WeaponsSystem
{
public:
    static void changeWeapon(entt::registry& registry, Weapons weapon);
    static void loadWeapon(entt::registry& registry, Weapons weapon, entt::entity ownerEntity);
    static void loadWeaponFromConfig(entt::registry& registry, entt::entity ownerEntity, Weapons weaponID);
    static void updateWeaponCooldown(entt::registry& registry, sf::Time deltaTime);
    static float getWeaponDamage(entt::registry& registry);
    static float getWeaponShotEnergyCost(entt::registry& registry);
    static float getWeaponSpecialShotEnergyCost(entt::registry& registry);
    static ShotFunction getWeaponShotFunction(ShotType shotType);
};