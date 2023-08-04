#include "CollisionSystem.h"

#include "EnemyInitializationSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../schema/EnemySchema.h"

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

    std::unordered_set<entt::entity> bulletsToDestroy;
    std::unordered_set<entt::entity> enemiesToDestroy;

    for (auto bullet : bullets) {
        if (bulletsToDestroy.find(bullet) != bulletsToDestroy.end())
        {
            continue;
        }

        auto& bulletCollision = bullets.get<Collision>(bullet);

        registry.view<Collision, Enemy>().each([&](auto enemy, auto& enemyCollision)
        {
            if (bulletsToDestroy.find(bullet) == bulletsToDestroy.end() &&
                enemiesToDestroy.find(enemy) == enemiesToDestroy.end() &&
                bulletCollision.collisionBox.intersects(enemyCollision.collisionBox))
            {
                bulletsToDestroy.insert(bullet);
                enemiesToDestroy.insert(enemy);

                //RESPAWN ONLY FOR TESTING
                EnemyInitializationSystem::createEnemy(registry, enemy2);
            }
        });
    }

    for (auto bullet : bulletsToDestroy)
    {
        registry.destroy(bullet);
    }

    for (auto enemy : enemiesToDestroy)
    {
        registry.destroy(enemy);
    }
}
