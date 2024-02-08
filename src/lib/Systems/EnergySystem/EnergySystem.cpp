#include "pch.h"
#include "EnergySystem.h"

#include "SkillSystem.h"

#include "player.h"

void EnergySystem::updateEnergy(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Player, Energy>();

    for (auto entity : view)
    {
        auto& energyComponent = view.get<Energy>(entity);

        if (energyComponent.currentEnergyValue < 0)
            energyComponent.currentEnergyValue = 0;

        if (energyComponent.regeneration)
        {
            if (energyComponent.currentEnergyValue < energyComponent.maxEnergyValue)
            {
                energyComponent.currentEnergyValue += energyComponent.energyRegenerationRate * SkillSystem::getValue(registry, SkillType::EnergyRegenerationMultiplierValue) * deltaTime.asSeconds();
                if (energyComponent.currentEnergyValue > energyComponent.maxEnergyValue)
                {
                    energyComponent.currentEnergyValue = energyComponent.maxEnergyValue;
                }
            }
        }
    }
}