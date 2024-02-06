#pragma once

#include <entt/entt.hpp>
#include <SFML/System.hpp>

#include "EventManager.h"
#include "SoundManager.h"

#include "uniqueTag.h"
#include "energy.h"

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
        auto& energyComponent = registry.get<Energy>(registry.view<Energy, EnergyOwnerTag>().front());
        
        if (energyComponent.currentEnergyValue > 0)
            return true;

        return false;
    }

    template<typename EnergyOwnerTag>
    requires UniqueTagConcept<EnergyOwnerTag>
    static bool hasEnoughEnergy(entt::registry& registry, float energyValue)
    {
        auto& energyComponent = registry.get<Energy>(registry.view<Energy, EnergyOwnerTag>().front());

        if (energyComponent.currentEnergyValue >= energyValue)
            return true;
        else
        {
            EventManager::getInstance().trigger(EventManager::Event::NotEnoughEnergy);
            SoundManager::getInstance().playSound("NotEnoughEnergy");
        }

        return false;
    }

    template <typename EnergyOwnerTag>
    requires UniqueTagConcept<EnergyOwnerTag>
    static void addEnergy(entt::registry& registry, float energyValue)
    {
        auto& energyComponent = registry.get<Energy>(registry.view<Energy, EnergyOwnerTag>().front());
        energyComponent.currentEnergyValue += energyValue;
        if (energyComponent.currentEnergyValue > energyComponent.maxEnergyValue)
            energyComponent.currentEnergyValue = energyComponent.maxEnergyValue;
    }

    template <typename EnergyOwnerTag>
    requires UniqueTagConcept<EnergyOwnerTag>
    static void removeEnergy(entt::registry& registry, float energyValue)
    {
        auto& energyComponent = registry.get<Energy>(registry.view<Energy, EnergyOwnerTag>().front());
        energyComponent.currentEnergyValue -= energyValue;
        if (energyComponent.currentEnergyValue < 0)
            energyComponent.currentEnergyValue = 0;
    }
};