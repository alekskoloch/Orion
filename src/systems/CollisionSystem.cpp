#include "CollisionSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void CollisionSystem::updateCollisionBoxes(entt::registry& registry)
{
    auto view = registry.view<Collision, Renderable>();
    for (auto entity : view)
    {
        auto& collision = view.get<Collision>(entity);
        auto& renderable = view.get<Renderable>(entity);

        collision.collisionBox = renderable.sprite.getGlobalBounds();
    }
}

void CollisionSystem::checkCollisions(entt::registry& registry)
{
    auto bullets = registry.view<Collision, Renderable, Velocity, Bullet>();
    auto enemies = registry.view<Collision, Enemy>();

    for (auto bullet : bullets)
    {
        auto& bulletCollision = bullets.get<Collision>(bullet);

        for (auto enemy : enemies)
        {
            auto& enemyCollision = enemies.get<Collision>(enemy);

            if (bulletCollision.collisionBox.intersects(enemyCollision.collisionBox))
            {
                registry.destroy(bullet);
                registry.destroy(enemy);
            }
        }
    }
}
