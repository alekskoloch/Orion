#include "ShieldSystem.h"

#include "../managers/TextureManager.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../utils/GraphicsOperations.h"

void ShieldSystem::restoreUnusedEnergy(Energy& energy, Shield& shield)
{
    energy.currentEnergyValue += shield.energyUsed;
        if (energy.currentEnergyValue > energy.maxEnergyValue)
            energy.currentEnergyValue = energy.maxEnergyValue;
}

void ShieldSystem::updateShieldPosition(entt::registry& registry)
{
    auto shieldView = registry.view<PlayerShield, Position>();
    for (auto entity : shieldView)
    {
        auto& shieldPosition = shieldView.get<Position>(entity);
        auto playerView = registry.view<Player, Position>();
        auto& playerPosition = playerView.get<Position>(playerView.front());

        shieldPosition.position = playerPosition.position;
    }
}

void ShieldSystem::shieldLoading(entt::registry& registry, Shield& shield, Energy& energy, sf::Time deltaTime)
{
    if (shield.energyUsed < shield.energyCost)
    {
        //TODO: Temporary energy regeneration management
        energy.energyRegenerationRate = 0;
        if (energy.currentEnergyValue > 0)
        {
            float correction = 0.f;
            shield.energyUsed += shield.energyCost * (deltaTime.asSeconds() / shield.loadTime);
            if (shield.energyUsed > shield.energyCost)
            {
                correction = shield.energyUsed - shield.energyCost;
                shield.energyUsed = shield.energyCost;
            }
            
            energy.currentEnergyValue -= shield.energyCost * (deltaTime.asSeconds() / shield.loadTime);
            if (energy.currentEnergyValue > 0)
                energy.currentEnergyValue += correction;

            if (energy.currentEnergyValue < 0)
                energy.currentEnergyValue = 0;
        }
    }
}

void ShieldSystem::interruptShieldLoading(entt::registry& registry, Input& input, Shield& shield, Energy& energy)
{
    energy.energyRegenerationRate = 10.f;
    ShieldSystem::restoreUnusedEnergy(energy, shield);
    shield.energyUsed = 0.f;
}

void ShieldSystem::shieldActivaton(entt::registry& registry, Shield& shield, Energy& energy)
{
    energy.energyRegenerationRate = 10.f;
    if (shield.energyUsed >= shield.energyCost)
        getShield(registry);
    shield.energyUsed = 0.f;
}

void ShieldSystem::handleShieldDuration(entt::registry& registry, Shield& shield, sf::Time deltaTime)
{
    shield.currentDuration -= deltaTime.asSeconds();
            
    if (shield.currentDuration <= 0)
    {
        shield.active = false;
        auto shieldView = registry.view<PlayerShield>();
        for (auto entity : shieldView)
        {
            registry.destroy(entity);
        }
    }
}

void ShieldSystem::changeShield(entt::registry& registry, ShieldSchema shield)
{
    auto player = registry.view<Player, Shield, Energy>();
    for (auto& entity : player)
    {
        auto& energy = player.get<Energy>(entity);
        auto& shieldComponent = player.get<Shield>(entity);

        ShieldSystem::restoreUnusedEnergy(energy, shieldComponent);
        shieldComponent.Set(shield);
    }
}

void ShieldSystem::updateShield(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Player, Shield, Input, Energy>();

    for (auto entity : view)
    {
        auto& input = view.get<Input>(entity);
        auto& shield = view.get<Shield>(entity);
        auto& energy = view.get<Energy>(entity);

        if (input.isGettingShield)
            ShieldSystem::shieldLoading(registry, shield, energy, deltaTime);
        else if (shield.energyUsed != 0 && !input.getShield)
            ShieldSystem::interruptShieldLoading(registry, input, shield, energy);

        if (shield.energyUsed >= shield.energyCost && input.getShield)
            ShieldSystem::shieldActivaton(registry, shield, energy);

        if (shield.active)
        {
            ShieldSystem::handleShieldDuration(registry, shield, deltaTime);
            ShieldSystem::updateShieldPosition(registry);
        }
    }

}

void ShieldSystem::getShield(entt::registry& registry)
{
    auto view = registry.view<Player, Shield>();

    for (auto entity : view)
    {
        auto& shield = view.get<Shield>(entity);
        
        if (shield.active)
        {
            auto shieldView = registry.view<PlayerShield>();
            for (auto entity : shieldView)
                registry.destroy(entity);
        }

        shield.currentDuration = shield.duration;
        shield.active = true;

        auto shieldEntity = registry.create();
        registry.emplace<PlayerShield>(shieldEntity);

        sf::Sprite shieldSprite = CreateSprite(shield.shieldTextureName);
        registry.emplace<Renderable>(shieldEntity, shieldSprite);
        registry.emplace<Position>(shieldEntity, registry.get<Position>(entity).position);
    }
}