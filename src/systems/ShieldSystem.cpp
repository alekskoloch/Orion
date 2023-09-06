#include "ShieldSystem.h"

#include "../managers/TextureManager.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void ShieldSystem::updateShield(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Player, Shield, Input>();

    for (auto entity : view)
    {
        auto& input = view.get<Input>(entity);
        auto& shield = view.get<Shield>(entity);

        if (input.getShield && !shield.active)
        {
           getShield(registry);
        }

        if (shield.active)
        {
            shield.duration -= deltaTime.asSeconds();
            
            if (shield.duration <= 0)
            {
                shield.active = false;
                auto shieldView = registry.view<PlayerShield>();
                for (auto entity : shieldView)
                {
                    registry.destroy(entity);
                }

                //TODO: Add shields shema
                shield.duration = 30.f;
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