#pragma once

#include "../pch.h"

class ShieldSystem
{
public:
    static void updateShield(entt::registry& registry, sf::Time deltaTime);
    static void getShield(entt::registry& registry);
};