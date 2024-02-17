#include "pch.h"
#include "HealthSystem.h"

void HealthSystem::updateHealth(entt::registry& registry)
{
    auto view = registry.view<Health, Removable>();
    for (auto entity : view)
    {
        auto& health = view.get<Health>(entity);
        if (health.currentHealthValue <= 0)
        {
            SoundManager::getInstance().playSound("Death");
            auto& removable = view.get<Removable>(entity);
            removable.remove = true;
        }
    }
}