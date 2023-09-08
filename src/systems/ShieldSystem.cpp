#include "ShieldSystem.h"

#include "../managers/TextureManager.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

//TODO: Make template function for loading schema
void ShieldSystem::loadShield(entt::registry& registry, const ShieldSchema& shieldSchema, entt::entity ownerEntity)
{
    TextureManager::getInstance().loadTexture(shieldSchema.shieldTextureName, ASSETS_PATH + shieldSchema.shieldTextureName + ".png");
    TextureManager::getInstance().loadTexture(shieldSchema.shieldIconTextureName, ASSETS_PATH + shieldSchema.shieldIconTextureName + ".png");
    registry.emplace<Shield>(ownerEntity, shieldSchema.durability, shieldSchema.energyCost, shieldSchema.duration, shieldSchema.loadTime, shieldSchema.shieldTextureName, shieldSchema.shieldIconTextureName);
}

void ShieldSystem::changeShield(entt::registry& registry, ShieldSchema shield)
{
    auto player = registry.view<Player, Shield, Energy>();
    for (auto& entity : player)
    {
        auto& energy = player.get<Energy>(entity);
        auto& shieldComponent = player.get<Shield>(entity);
        shieldComponent.durability = shield.durability;
        shieldComponent.energyCost = shield.energyCost;
        shieldComponent.duration = shield.duration;
        shieldComponent.loadTime = shield.loadTime;

        energy.currentEnergyValue += shieldComponent.energyUsed;
        shieldComponent.energyUsed = 0.f;
        shieldComponent.currentLoadTime = 0.f;
        shieldComponent.currentDuration = 0.f;

        TextureManager::getInstance().loadTexture(shield.shieldTextureName, ASSETS_PATH + shield.shieldTextureName + ".png");
        shieldComponent.shieldTextureName = shield.shieldTextureName;
        TextureManager::getInstance().loadTexture(shield.shieldIconTextureName, ASSETS_PATH + shield.shieldIconTextureName + ".png");
        shieldComponent.shieldIconTextureName = shield.shieldIconTextureName;
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
        {
            if (shield.currentLoadTime < shield.loadTime)
            {
                //TODO: Temporary energy regeneration management
                energy.energyRegenerationRate = 0;
                if (energy.currentEnergyValue > 0)
                {
                    shield.energyUsed += shield.energyCost * (deltaTime.asSeconds() / shield.loadTime);
                    energy.currentEnergyValue -= shield.energyCost * (deltaTime.asSeconds() / shield.loadTime);
                    if (energy.currentEnergyValue < 0)
                        energy.currentEnergyValue = 0;
                }
                shield.currentLoadTime += deltaTime.asSeconds();
            }
        }
        else if (shield.currentLoadTime != 0 && !input.getShield)
        {
            energy.energyRegenerationRate = 10.f;
            energy.currentEnergyValue += shield.energyUsed;
            if (energy.currentEnergyValue > energy.maxEnergyValue)
                energy.currentEnergyValue = energy.maxEnergyValue;
            shield.energyUsed = 0.f;
            shield.currentLoadTime = 0;
        }

        if (shield.currentLoadTime >= shield.loadTime && input.getShield)
        {
            energy.energyRegenerationRate = 10.f;
            shield.currentLoadTime = 0;
            if (shield.energyUsed >= shield.energyCost)
            {
                getShield(registry);
            }
            shield.energyUsed = 0.f;
        }

        if (shield.active)
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
    }

    //update shield position to player position
    auto shieldView = registry.view<PlayerShield, Position>();
    for (auto entity : shieldView)
    {
        auto& shieldPosition = shieldView.get<Position>(entity);
        auto playerView = registry.view<Player, Position>();
        auto& playerPosition = playerView.get<Position>(playerView.front());

        shieldPosition.position = playerPosition.position;
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
            {
                registry.destroy(entity);
            }
        }

        shield.currentDuration = shield.duration;

        shield.active = true;

        //creating shield

        auto shieldEntity = registry.create();
        registry.emplace<PlayerShield>(shieldEntity);

        sf::Sprite shieldSprite(TextureManager::getInstance().getTexture(shield.shieldTextureName));
        shieldSprite.setOrigin(shieldSprite.getGlobalBounds().width / 2.f, shieldSprite.getGlobalBounds().height / 2.f);
        registry.emplace<Renderable>(shieldEntity, shieldSprite);

        registry.emplace<Position>(shieldEntity, registry.get<Position>(entity).position);
    }
}