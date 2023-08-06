#include "CollisionSystem.h"

#include "EnemyInitializationSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../schema/EnemySchema.h"

void CollisionSystem::updateCollisionBoxes(entt::registry& registry)
{
    auto view = registry.view<Collision, Position>();
    for (auto entity : view)
    {
        auto& collision = view.get<Collision>(entity);
        auto& position = view.get<Position>(entity);

        collision.collisionBox.left = position.position.x - collision.collisionBox.width / 2.f;
        collision.collisionBox.top = position.position.y - collision.collisionBox.height / 2.f;
    }
}

void CollisionSystem::checkCollisions(entt::registry& registry)
{
    auto playerBullets = registry.view<Collision, Renderable, Velocity, Bullet, PlayerBullet>();
    auto enemies = registry.view<Collision, Enemy>();
    auto enemyBullets = registry.view<Collision, Renderable, Velocity, Bullet, EnemyBullet>();
    auto players = registry.view<Collision, Player>();

    std::unordered_set<entt::entity> bulletsToDestroy;
    std::unordered_set<entt::entity> enemiesToDestroy;
    std::unordered_set<entt::entity> playersToDestroy;

    for (auto bullet : playerBullets)
    {
        if (bulletsToDestroy.find(bullet) != bulletsToDestroy.end())
        {
            continue;
        }

        auto& bulletCollision = playerBullets.get<Collision>(bullet);

        enemies.each([&](auto enemy, auto& enemyCollision)
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

    for (auto bullet : enemyBullets)
    {
        if (bulletsToDestroy.find(bullet) != bulletsToDestroy.end())
        {
            continue;
        }

        auto& bulletCollision = enemyBullets.get<Collision>(bullet);

        players.each([&](auto player, auto& playerCollision)
        {
            if (bulletsToDestroy.find(bullet) == bulletsToDestroy.end() &&
                playersToDestroy.find(player) == playersToDestroy.end() &&
                bulletCollision.collisionBox.intersects(playerCollision.collisionBox))
            {
                bulletsToDestroy.insert(bullet);
                //TODO: handle player death
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

    for (auto player : playersToDestroy)
    {
        registry.destroy(player);
    }
}
