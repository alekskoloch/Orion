#pragma once

#include "../pch.h"

#include <type_traits>

struct UniqueTag {};

struct Player : UniqueTag {};

template <typename Tag>
concept UniqueTagConcept = std::is_base_of_v<UniqueTag, Tag>;

struct Enemy{};

struct Bullet {};
struct PlayerBullet{};
struct EnemyBullet{};

struct PlayerShield {};

struct DropItem { std::string name; };