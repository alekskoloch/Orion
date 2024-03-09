#include "pch.h"
#include "EnemyBuilder.h"

#include "WaypointsMovementSystem.h"

#include "removable.h"
#include "entityState.h"
#include "name.h"
#include "position.h"
#include "speed.h"
#include "health.h"
#include "experience.h"
#include "drop.h"
#include "waypointMovement.h"
#include "enemy.h"

EnemyBuilder::EnemyBuilder(entt::registry& registry) : registry(registry)
{
    
}

EnemyBuilder& EnemyBuilder::createEnemy(std::string name)
{
    enemy = registry.create();

    registry.emplace<Enemy>(enemy);
    registry.emplace<Name>(enemy, name);
    
    sf::Sprite sprite = CreateSprite(name);
    registry.emplace<Collision>(enemy, sprite.getGlobalBounds());
    registry.emplace<Renderable>(enemy, sprite);
    registry.emplace<Removable>(enemy, false);

    return *this;
}

EnemyBuilder& EnemyBuilder::addEntityState(float attackRange, float idleRange)
{
    registry.emplace<EntityState>(enemy);
    auto& enemyState = registry.get<EntityState>(enemy);
    enemyState.attackRange = attackRange;
    enemyState.idleRange = idleRange;

    return *this;
}

EnemyBuilder& EnemyBuilder::addPosition(sf::Vector2f position)
{
    registry.emplace<Position>(enemy, position);

    return *this;
}

EnemyBuilder& EnemyBuilder::addSpeed(float speed)
{
    registry.emplace<Speed>(enemy, speed);
    registry.emplace<Velocity>(enemy, sf::Vector2f(0.f, 0.f));

    return *this;
}

EnemyBuilder& EnemyBuilder::addHealth(float health)
{
    registry.emplace<Health>(enemy, health, health);

    return *this;
}

EnemyBuilder& EnemyBuilder::addExperience(float experience)
{
    //TODO: Level is temporary
    registry.emplace<Experience>(enemy, 1, experience);

    return *this;
}

EnemyBuilder& EnemyBuilder::addDrop(int dropChance, int dropRate)
{
    registry.emplace<Drop>(enemy, dropChance, dropRate);

    return *this;
}

EnemyBuilder& EnemyBuilder::setWeapon(Weapons weapon)
{
    WeaponsSystem::loadWeapon(this->registry, weapon, this->enemy);

    return *this;
}

EnemyBuilder& EnemyBuilder::setWaypointMovement()
{
    registry.emplace<WaypointMovement>(enemy, WaypointsMovementSystem::getRandomWaypointsNearPosition(registry.get<Position>(enemy).position));

    return *this;
}