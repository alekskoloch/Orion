#pragma once

#include "../components/components.h"
#include "WeaponsSchema.h"

struct EnemySchema
{
    std::string textureName;
    float health;
    sf::Vector2f position;
    float speed;
    unsigned int experience;
    std::vector<sf::Vector2f> waypoints;
    std::string name;
    WeaponSchema weaponSchema;
    float attackRange;
    float idleRange;
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
};

static EnemySchema enemy =
{
    "enemy1",
    2.f,
    sf::Vector2f(100.f, 980.f),
    600.f,
    10u,
    {
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f)
    },
    "enemy",
    basicEnemyWeapon,
    1000.f,
    2000.f
};

static EnemySchema enemy2 =
{
    "enemy2",
    2.f,
    sf::Vector2f(100.f, 100.f),
    500.f,
    10u,
    {
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f),
        sf::Vector2f(100.f, 100.f)
    },
    "enemy2",
    basicEnemyWeapon2,
    600.f,
    1000.f
};

static EnemySchema enemy3 =
{
    "enemy3",
    2.f,
    sf::Vector2f(1820.f, 100.f),
    400.f,
    14u,
    {
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f),
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f)
    },
    "enemy3",
    basicEnemyWeapon3,
    800.f,
    1500.f
};

static EnemySchema enemy4 =
{
    "enemy4",
    2.f,
    sf::Vector2f(1820.f, 980.f),
    300.f,
    13u,
    {
        sf::Vector2f(100.f, 980.f),
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f)
    },
    "enemy4",
    basicEnemyWeapon4,
    1400.f,
    1800.f
};

static EnemySchema enemyRed1 =
{
    "enemy_red_1",
    3.f,
    sf::Vector2f(100.f, 980.f),
    200.f,
    16u,
    {
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f)
    },
    "enemy_red_1",
    Shuriken,
    2000.f,
    2500.f
};

static EnemySchema enemyRed2 =
{
    "enemy_red_2",
    3.f,
    sf::Vector2f(100.f, 100.f),
    500.f,
    16u,
    {
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f),
        sf::Vector2f(100.f, 100.f)
    },
    "enemy_red_2",
    Shuriken,
    1500.f,
    2000.f
};

static EnemySchema enemyRed3 =
{
    "enemy_red_3",
    3.f,
    sf::Vector2f(1820.f, 100.f),
    600.f,
    22u,
    {
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f),
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f)
    },
    "enemy_red_3",
    DoubleShuriken,
    1700.f,
    1800.f
};

static EnemySchema enemyRed4 =
{
    "enemy_red_4",
    3.f,
    sf::Vector2f(1820.f, 980.f),
    700.f,
    22u,
    {
        sf::Vector2f(100.f, 980.f),
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f)
    },
    "enemy_red_4",
    DoubleShuriken,
    1300.f,
    1600.f
};

static EnemySchema enemyZombie1 =
{
    "enemy_zombie_1",
    3.f,
    sf::Vector2f(100.f, 980.f),
    200.f,
    14u,
    {
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f)
    },
    "enemy_zombie_1",
    Nail,
    1300.f,
    1600.f
};

static EnemySchema enemyZombie2 =
{
    "enemy_zombie_2",
    5.f,
    sf::Vector2f(100.f, 980.f),
    300.f,
    14u,
    {
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f)
    },
    "enemy_zombie_2",
    Nail,
    1500.f,
    1800.f
};

static EnemySchema enemyZombie3 =
{
    "enemy_zombie_3",
    2.f,
    sf::Vector2f(100.f, 980.f),
    900.f,
    16u,
    {
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f)
    },
    "enemy_zombie_3",
    Nail,
    1500.f,
    1900.f
};

static EnemySchema enemyZombie4 =
{
    "enemy_zombie_4",
    5.f,
    sf::Vector2f(100.f, 980.f),
    500.f,
    17u,
    {
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f)
    },
    "enemy_zombie_4",
    Nail,
    1500.f,
    2200.f
};

static EnemySchema enemyZombie5 =
{
    "enemy_zombie_5",
    2.f,
    sf::Vector2f(100.f, 980.f),
    900.f,
    19u,
    {
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f)
    },
    "enemy_zombie_5",
    Nail,
    1500.f,
    1900.f
};