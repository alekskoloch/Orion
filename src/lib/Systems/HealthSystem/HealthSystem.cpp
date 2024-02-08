#include "pch.h"
#include "HealthSystem.h"

void HealthSystem::updateHealth(entt::registry& registry)
{
    auto view = registry.view<Health>();
    for (auto entity : view)
    {
        auto& health = view.get<Health>(entity);
        if (health.currentHealthValue <= 0)
        {
            SoundManager::getInstance().playSound("Death");
            registry.destroy(entity);
        }
    }
}