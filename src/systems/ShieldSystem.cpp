#include "ShieldSystem.h"

#include "../managers/TextureManager.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void ShieldSystem::getShield(entt::registry& registry)
{
    auto view = registry.view<Player, Shield, Energy>();

    for (auto entity : view)
    {
        auto& energy = view.get<Energy>(entity);
        auto& shield = view.get<Shield>(entity);
        
        if (energy.currentEnergyValue >= shield.energyCost)
        {
            energy.currentEnergyValue -= shield.energyCost;

            shield.active = true;

            //creating shield

            auto shieldEntity = registry.create();
            registry.emplace<PlayerShield>(shieldEntity);

            TextureManager::getInstance().loadTexture("BASIC_SHIELD", ASSETS_PATH + std::string("shield") + ".png");
            sf::Sprite shieldSprite(TextureManager::getInstance().getTexture("BASIC_SHIELD"));
            shieldSprite.setOrigin(shieldSprite.getGlobalBounds().width / 2.f, shieldSprite.getGlobalBounds().height / 2.f);
            registry.emplace<Renderable>(shieldEntity, shieldSprite);

            registry.emplace<Position>(shieldEntity, registry.get<Position>(entity).position);
        }
    }
}