#pragma once

#include "../pch.h"

#include "shield.h"

#include "../schema/ShieldSchema.h"

class ShieldSystem
{
public:
    static void loadShield(entt::registry& registry, const ShieldSchema& shieldSchema, entt::entity ownerEntity);
    static void changeShield(entt::registry& registry, ShieldSchema shield);
    static void updateShield(entt::registry& registry, sf::Time deltaTime);
    static void getShield(entt::registry& registry);
    static void getShield(entt::registry& registry, ShieldSchema shieldSchema);
    static void destroyShield(entt::registry& registry);

private:
    static void updateShieldPosition(entt::registry& registry);

    static void shieldLoading(entt::registry& registry, Shield& shield, sf::Time deltaTime);
    static void interruptShieldLoading(entt::registry& registry, Shield& shield);
    static void shieldActivaton(entt::registry& registry, Shield& shield);
    static void handleShieldDuration(entt::registry& registry, Shield& shield, sf::Time deltaTime);
};