#pragma once

#include "../pch.h"

#include "../components/components.h"

#include "../schema/ShieldSchema.h"

class ShieldSystem
{
public:
    static void loadShield(entt::registry& registry, const ShieldSchema& shieldSchema, entt::entity ownerEntity);
    static void changeShield(entt::registry& registry, ShieldSchema shield);
    static void updateShield(entt::registry& registry, sf::Time deltaTime);
    static void getShield(entt::registry& registry);

private:
    static void restoreUnusedEnergy(Energy& energy, Shield& shield);
    static void updateShieldPosition(entt::registry& registry);

    static void shieldLoading(entt::registry& registry, Shield& shield, sf::Time deltaTime);
    static void interruptShieldLoading(entt::registry& registry, Input& input, Shield& shield, Energy& energy);
    static void shieldActivaton(entt::registry& registry, Shield& shield, Energy& energy);
    static void handleShieldDuration(entt::registry& registry, Shield& shield, sf::Time deltaTime);
};