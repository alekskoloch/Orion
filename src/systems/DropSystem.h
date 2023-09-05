#pragma once

#include "../pch.h"

class DropSystem
{
public:
    static void drop(entt::registry& registry, entt::entity& entity);
    static void updateDrop(entt::registry& registry, sf::Time deltaTime);
};