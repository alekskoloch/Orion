#pragma once

#include "../pch.h"

#include "../schema/ShieldSchema.h"

class ShieldSystem
{
public:
    static void loadShield(entt::registry& registry, const ShieldSchema& shieldSchema, entt::entity ownerEntity);
    static void changeShield(entt::registry& registry, ShieldSchema shield);
    static void updateShield(entt::registry& registry, sf::Time deltaTime);
    static void getShield(entt::registry& registry);
};