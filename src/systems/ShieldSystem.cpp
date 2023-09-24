#include "ShieldSystem.h"

#include "../managers/TextureManager.h"

#include "../systems/EnergySystem.h"
#include "../systems/SkillSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../utils/GraphicsOperations.h"

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

void ShieldSystem::shieldLoading(entt::registry& registry, Shield& shield, sf::Time deltaTime)
{
    if (shield.energyUsed < shield.energyCost)
    {
        EnergySystem::disableEnergyRegeneration<Player>(registry);
        if (EnergySystem::hasAnyEnergy<Player>(registry))
        {
            float correction = 0.f;
            shield.energyUsed += shield.energyCost * (deltaTime.asSeconds() / shield.loadTime);
            if (shield.energyUsed > shield.energyCost)
            {
                correction = shield.energyUsed - shield.energyCost;
                shield.energyUsed = shield.energyCost;
            }
            
            EnergySystem::removeEnergy<Player>(registry, shield.energyCost * (deltaTime.asSeconds() / shield.loadTime));
            if (EnergySystem::hasAnyEnergy<Player>(registry))
                EnergySystem::addEnergy<Player>(registry, correction);
        }
    }
}

void ShieldSystem::interruptShieldLoading(entt::registry& registry, Shield& shield)
{
    EnergySystem::enableEnergyRegeneration<Player>(registry);
    EnergySystem::addEnergy<Player>(registry, shield.energyUsed);
    shield.energyUsed = 0.f;
}

void ShieldSystem::shieldActivaton(entt::registry& registry, Shield& shield)
{
    EnergySystem::enableEnergyRegeneration<Player>(registry);
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
    auto player = registry.view<Player, Shield>();
    for (auto& entity : player)
    {
        auto& shieldComponent = player.get<Shield>(entity);

        EnergySystem::addEnergy<Player>(registry, shieldComponent.energyUsed);
        shieldComponent.Set(shield);
    }
}

void ShieldSystem::updateShield(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Player, Shield, Input>();

    for (auto entity : view)
    {
        auto& input = view.get<Input>(entity);
        auto& shield = view.get<Shield>(entity);

        if (input.isGettingShield)
            ShieldSystem::shieldLoading(registry, shield, deltaTime);
        else if (shield.energyUsed != 0 && !input.getShield)
            ShieldSystem::interruptShieldLoading(registry, shield);

        if (shield.energyUsed >= shield.energyCost && input.getShield)
            ShieldSystem::shieldActivaton(registry, shield);

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

        shield.currentDuration = shield.duration * SkillSystem::getShieldTimeDurationMultiplier(registry);
        shield.active = true;

        auto shieldEntity = registry.create();
        registry.emplace<PlayerShield>(shieldEntity);

        sf::Sprite shieldSprite = CreateSprite(shield.shieldTextureName);
        registry.emplace<Renderable>(shieldEntity, shieldSprite);
        registry.emplace<Position>(shieldEntity, registry.get<Position>(entity).position);
    }
}

void ShieldSystem::getShield(entt::registry& registry, ShieldSchema shieldSchema)
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

        shield.currentDuration = shieldSchema.duration * SkillSystem::getShieldTimeDurationMultiplier(registry);
        shield.active = true;

        auto shieldEntity = registry.create();
        registry.emplace<PlayerShield>(shieldEntity);

        sf::Sprite shieldSprite = CreateSprite(shieldSchema.shieldTextureName);
        registry.emplace<Renderable>(shieldEntity, shieldSprite);
        registry.emplace<Position>(shieldEntity, registry.get<Position>(entity).position);
    }
}