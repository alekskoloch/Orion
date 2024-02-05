#pragma once

#include <SFML/Graphics.hpp> 
#include <type_traits>

struct UniqueTag {};

struct Player : UniqueTag {};

template <typename Tag>
concept UniqueTagConcept = std::is_base_of_v<UniqueTag, Tag>;

struct Enemy{};

struct Bullet {};
struct PlayerBullet{};
struct EnemyBullet{};

struct ShurikenBullet {
    sf::Vector2f enemyPositionWhenShot;
    sf::Vector2f playerPositionWhenShot;
    bool right;
    float speed;
    float currentTime = 0.f;
};

struct PlayerShield {};

struct DropItem { std::string name; };