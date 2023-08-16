#pragma once

#include "../components/components.h"
#include "WeaponsSchema.h"

struct EnemySchema
{
    std::string textureName;
    sf::Vector2f position;
    float speed;
    std::vector<sf::Vector2f> waypoints;
    std::string name;
    WeaponSchema weaponSchema;
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
};

static EnemySchema enemy =
{
    "enemy",
    sf::Vector2f(100.f, 980.f),
    600.f,
    {
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f)
    },
    "enemy",
    basicEnemyWeapon
};

static EnemySchema enemy2 =
{
    "enemy2",
    sf::Vector2f(100.f, 100.f),
    500.f,
    {
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f),
        sf::Vector2f(100.f, 100.f)
    },
    "enemy2",
    basicEnemyWeapon2
};

static EnemySchema enemy3 =
{
    "enemy3",
    sf::Vector2f(1820.f, 100.f),
    400.f,
    {
        sf::Vector2f(1820.f, 980.f),
        sf::Vector2f(100.f, 980.f),
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f)
    },
    "enemy3",
    basicEnemyWeapon3
};

static EnemySchema enemy4 =
{
    "enemy4",
    sf::Vector2f(1820.f, 980.f),
    300.f,
    {
        sf::Vector2f(100.f, 980.f),
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(1820.f, 100.f),
        sf::Vector2f(1820.f, 980.f)
    },
    "enemy4",
    basicEnemyWeapon4
};