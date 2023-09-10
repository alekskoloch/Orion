#pragma once

#include "../pch.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

class EnergySystem
{
public:
    static void updateEnergy(entt::registry& registry, sf::Time deltaTime);

    template<typename EnergyOwnerTag>
    static void enableEnergyRegeneration(entt::registry& registry)
    {
        auto view = registry.view<Energy, EnergyOwnerTag>();

        for (auto entity : view)
        {
            auto& energyComponent = view.template get<Energy>(entity);
            energyComponent.regeneration = true;
        }
    }

    template<typename EnergyOwnerTag>
    static void disableEnergyRegeneration(entt::registry& registry)
    {
        auto view = registry.view<Energy, EnergyOwnerTag>();

        for (auto entity : view)
        {
            auto& energyComponent = view.template get<Energy>(entity);
            energyComponent.regeneration = false;
        }
    }

    template<typename EnergyOwnerTag>
    requires UniqueTagConcept<EnergyOwnerTag>
    static bool hasAnyEnergy(entt::registry& registry)
    {
        auto view = registry.view<Energy, EnergyOwnerTag>();

        for (auto entity : view)
        {
            auto& energyComponent = view.template get<Energy>(entity);
            if (energyComponent.currentEnergyValue > 0)
                return true;
        }

        return false;
    }
};