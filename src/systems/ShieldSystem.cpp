#include "ShieldSystem.h"

#include "../managers/TextureManager.h"
#include "../managers/SoundManager.h"

#include "../systems/EnergySystem.h"
#include "../systems/SkillSystem.h"
#include "../systems/CooldownSystem.h"

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
        //TODO: this sound effect is temporary, it should be replaced with a better one
        if (SoundManager::getInstance().isLoopedSoundPlaying("ShieldLoading") == false && EnergySystem::hasAnyEnergy<Player>(registry))
            SoundManager::getInstance().playLoopedSound("ShieldLoading");
        EnergySystem::disableEnergyRegeneration<Player>(registry);
        if (EnergySystem::hasAnyEnergy<Player>(registry))
        {
            float correction = 0.f;
            shield.energyUsed += shield.energyCost * (deltaTime.asSeconds() / shield.loadTime);
            if (shield.energyUsed > shield.energyCost)
            {
                correction = shield.energyUsed - shield.energyCost;
                shield.energyUsed = shield.energyCost;
                SoundManager::getInstance().stopLoopedSound("ShieldLoading");
                SoundManager::getInstance().playSound("ShieldUp");
                ShieldSystem::shieldActivaton(registry, shield);

                auto playerView = registry.view<Player, Input>();
                auto& playerInput = playerView.get<Input>(playerView.front());
                playerInput.canGetShield = false;
            }
            
            EnergySystem::removeEnergy<Player>(registry, shield.energyCost * (deltaTime.asSeconds() / shield.loadTime));
            if (EnergySystem::hasAnyEnergy<Player>(registry))
                EnergySystem::addEnergy<Player>(registry, correction);
        }
        else
        {
            SoundManager::getInstance().stopLoopedSound("ShieldLoading");
            ShieldSystem::interruptShieldLoading(registry, shield);
        }
    }
    else
    {
        SoundManager::getInstance().stopLoopedSound("ShieldLoading");
    }
}

void ShieldSystem::interruptShieldLoading(entt::registry& registry, Shield& shield)
{
    EnergySystem::enableEnergyRegeneration<Player>(registry);
    EnergySystem::addEnergy<Player>(registry, shield.energyUsed);
    SoundManager::getInstance().playSound("ShieldInterrupt");
    shield.energyUsed = 0.f;

    auto playerView = registry.view<Player, Input>();
    auto& playerInput = playerView.get<Input>(playerView.front());
    playerInput.canGetShield = false;
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
    if (CooldownSystem::getCooldown(registry, registry.view<Player>().front(), "shieldCooldown") == 0.f)
    {
        shield.active = false;
        destroyShield(registry);
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

        if (input.isGettingShield && input.canGetShield)
        {
            ShieldSystem::shieldLoading(registry, shield, deltaTime);
        }
        else if (shield.energyUsed != 0 && !input.getShield)
        {
            ShieldSystem::interruptShieldLoading(registry, shield);
            SoundManager::getInstance().stopLoopedSound("ShieldLoading");
        }

        // if (shield.energyUsed >= shield.energyCost && input.getShield)
        // {
        //     ShieldSystem::shieldActivaton(registry, shield);
        // }

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
            ShieldSystem::destroyShield(registry);
        }

        CooldownSystem::setCooldown(registry, entity, "shieldCooldown", shield.duration * SkillSystem::getValue(registry, SkillType::ShieldTimeDurationMultiplierValue));
    
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
            ShieldSystem::destroyShield(registry);
        }

        CooldownSystem::setCooldown(registry, entity, "shieldCooldown", shield.duration * SkillSystem::getValue(registry, SkillType::ShieldTimeDurationMultiplierValue));

        shield.active = true;

        auto shieldEntity = registry.create();
        registry.emplace<PlayerShield>(shieldEntity);

        sf::Sprite shieldSprite = CreateSprite(shieldSchema.shieldTextureName);
        registry.emplace<Renderable>(shieldEntity, shieldSprite);
        registry.emplace<Position>(shieldEntity, registry.get<Position>(entity).position);
    }
}

void ShieldSystem::destroyShield(entt::registry& registry)
{
    auto view = registry.view<PlayerShield>();
    for (auto entity : view)
        registry.destroy(entity);
}