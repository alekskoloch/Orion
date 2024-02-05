#include "CooldownSystem.h"

#include "cooldown.h"

void CooldownSystem::updateCooldowns(entt::registry& registry, sf::Time deltaTime)
{
    auto cooldownsView = registry.view<Cooldown>();

    for (auto entity : cooldownsView)
    {
        auto& cooldownComponent = cooldownsView.get<Cooldown>(entity);

        //cooldown in cooldownComponent is unordered map with cooldown name and time left, so we need to iterate through it
        for (auto& cooldown : cooldownComponent.cooldowns)
        {
            if (cooldown.second > 0)
            {
                cooldown.second -= deltaTime.asSeconds();
                if (cooldown.second < 0)
                    cooldown.second = 0;
            }
        }
    }
}

void CooldownSystem::setCooldown(entt::registry& registry, entt::entity entity, std::string cooldownName, float cooldownTime)
{
    auto cooldownsView = registry.view<Cooldown>();

    auto& cooldownComponent = cooldownsView.get<Cooldown>(entity);

    cooldownComponent.cooldowns[cooldownName] = cooldownTime;
}

float CooldownSystem::getCooldown(entt::registry& registry, entt::entity entity, std::string cooldownName)
{
    auto cooldownsView = registry.view<Cooldown>();

    auto& cooldownComponent = cooldownsView.get<Cooldown>(entity);

    if (cooldownComponent.cooldowns.find(cooldownName) == cooldownComponent.cooldowns.end())
        return -1.f;
    else
        return cooldownComponent.cooldowns[cooldownName];
}