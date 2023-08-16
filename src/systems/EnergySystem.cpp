#include "EnergySystem.h"

#include "../components/components.h"

void EnergySystem::updateEnergy(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Energy>();

    for (auto entity : view)
    {
        auto& energyComponent = view.get<Energy>(entity);

        if (energyComponent.maxEnergyValue < energyComponent.currentEnergyValue)
        {
            energyComponent.currentEnergyValue += energyComponent.energyRegenerationRate * deltaTime.asSeconds();
            if (energyComponent.currentEnergyValue > energyComponent.maxEnergyValue)
            {
                energyComponent.currentEnergyValue = energyComponent.maxEnergyValue;
            }
        }
    }
}