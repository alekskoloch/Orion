#pragma once

//TODO: This is horrible, shot implementation should be moved to a system
#include "../Systems/BulletSystem/BulletSystem.h"
#include "../Systems/ProceduralGenerationSystem/ProceduralGenerationSystem.h"

#include "playerBullet.h"
#include "enemyBullet.h"
#include "shurikenBullet.h"
#include "player.h"

//TODO: only temporary
#include <iostream>

enum class SpecialShotType
{
    FullCircleShoot,
    TripleSalvo,
    None
};

struct WeaponSchema
{
    WeaponType weaponType;
    float damage;
    float cooldownTime;
    float bulletSpeed;
    std::string bulletTextureName;
    std::string weaponIconTextureName;
    std::function<void(entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)> shoot = [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity) {};
    std::function<void(entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)> specialShoot = [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity) {};
    SpecialShotType specialShotType = SpecialShotType::None;
    float energyCost = 0.f;
    float energyCostForSpecialShot = 0.f;
    float specialShotCooldownTime = 0.f;
    unsigned bulletsInSalvo = 0;
    float queueCooldown = 0.0f;
};

static WeaponSchema redWeapon =
{
    WeaponType::SingleShot,
    1.f,
    0.2f,
    1300.f,
    "red_bullet",
    "red_weapon",
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        sf::Vector2f targetPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        BulletSystem::createBullet<PlayerBullet>(registry, entity, targetPosition);
    },
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    { 
        sf::Vector2f targetPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        float amgleOffset[] = { 0.f, 45.f, 90.f, 135.f, 180.f, 225.f, 270.f, 315.f };

        for (auto offset : amgleOffset)
            BulletSystem::createBullet<PlayerBullet>(registry, entity, targetPosition, false, offset);
    },
    SpecialShotType::FullCircleShoot,
    10.f,
    100.f,
    5.f
};

static WeaponSchema blueWeapon =
{
    WeaponType::TrippleShot,
    1.f,
    0.4f,
    2000.f,
    "blue_bullet",
    "blue_weapon",
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        sf::Vector2f targetPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        float angleOffset[] = { -10.f, 0.f, 10.f };

        for (auto offset : angleOffset)
            BulletSystem::createBullet<PlayerBullet>(registry, entity, targetPosition, false, offset);
    },
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        auto& playerWeapon = registry.get<Weapon>(entity);

        playerWeapon.bulletsInQueue = playerWeapon.bulletsInSalvo;
    },
    SpecialShotType::TripleSalvo,
    35.f,
    100.f,
    5.f,
    5,
    0.03f
};


static WeaponSchema basicEnemyWeapon =
{
    WeaponType::SingleShot,
    1.f,
    2.5f,
    800.f,
    "enemy_bullet",
    "empty",
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        auto playerView = registry.view<Player>();
        auto playerEntity = playerView.front();

        //target position = player position
        sf::Vector2f targetPosition = registry.get<Position>(playerEntity).position;

        BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition);
    },
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        std::cout << "basic enemy weapon special shoot" << std::endl;
    }
};

static WeaponSchema basicEnemyWeapon2 =
{
    WeaponType::TrippleShot,
    1.f,
    4.0f,
    500.f,
    "enemy_bullet2",
    "empty",
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        auto playerView = registry.view<Player>();
        auto playerEntity = playerView.front();

        sf::Vector2f targetPosition = registry.get<Position>(playerEntity).position;

        float angleOffset[] = { -10.f, 0.f, 10.f };

        for (auto offset : angleOffset)
            BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition, false, offset);
    },
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        std::cout << "basic enemy weapon special shoot" << std::endl;
    }
};

static WeaponSchema basicEnemyWeapon3 =
{
    WeaponType::SingleShot,
    1.f,
    5.0f,
    2000.f,
    "enemy_bullet3",
    "empty",
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        auto playerView = registry.view<Player>();
        auto playerEntity = playerView.front();

        //target position = player position
        sf::Vector2f targetPosition = registry.get<Position>(playerEntity).position;

        BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition);
    },
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        std::cout << "basic enemy weapon special shoot" << std::endl;
    }
};

static WeaponSchema basicEnemyWeapon4 =
{
    WeaponType::TrippleShot,
    1.f,
    2.0f,
    800.f,
    "enemy_bullet4",
    "empty",
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        auto playerView = registry.view<Player>();
        auto playerEntity = playerView.front();

        sf::Vector2f targetPosition = registry.get<Position>(playerEntity).position;

        float angleOffset[] = { -10.f, 0.f, 10.f };

        for (auto offset : angleOffset)
            BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition, false, offset);
    },
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        std::cout << "basic enemy weapon special shoot" << std::endl;
    }
};

static WeaponSchema Shuriken =
{
    WeaponType::Shuriken,
    1.f,
    2.0f,
    1800.f,
    "enemy_red_bullet",
    "empty",
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        auto playerView = registry.view<Player>();
        auto playerEntity = playerView.front();

        sf::Vector2f targetPosition = registry.get<Position>(playerEntity).position;

        BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition, ProceduralGenerationSystem::GetRandomNumber(0, 1) == 1);
    },
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        std::cout << "shuriken weapon special shoot" << std::endl;
    }
};

static WeaponSchema DoubleShuriken =
{
    WeaponType::DoubleShuriken,
    1.f,
    3.0f,
    1800.f,
    "enemy_red_bullet",
    "empty",
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        auto playerView = registry.view<Player>();
        auto playerEntity = playerView.front();

        sf::Vector2f targetPosition = registry.get<Position>(playerEntity).position;

        BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition, true);
        BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition, false);
    },
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        std::cout << "double shuriken weapon special shoot" << std::endl;
    }
};

static WeaponSchema Nail =
{
    WeaponType::SingleShot,
    1.f,
    1.5f,
    1000.f,
    "enemy_zombie_bullet",
    "empty",
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        auto playerView = registry.view<Player>();
        auto playerEntity = playerView.front();

        //target position = player position
        sf::Vector2f targetPosition = registry.get<Position>(playerEntity).position;

        BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition);
    },
    [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
    {
        std::cout << "nail weapon special shoot" << std::endl;
    }
};
