#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

//TODO: This is horrible, this should be handled by the weapon system
#include "../Systems/WeaponSystem/WeaponsSystem.h"

class EnemyBuilder
{
public:
    EnemyBuilder(entt::registry& registry);

    EnemyBuilder& createEnemy(std::string name);
    EnemyBuilder& addEntityState(float attackRange, float idleRange);
    EnemyBuilder& addPosition(sf::Vector2f position);
    EnemyBuilder& addSpeed(float speed);
    EnemyBuilder& addHealth(float health);
    EnemyBuilder& addExperience(float experience);
    EnemyBuilder& addDrop(int dropChance, int dropRate);
    EnemyBuilder& setWeapon(Weapons weapon);
    EnemyBuilder& setWaypointMovement();

private:
    entt::registry& registry;
    entt::entity enemy;
};