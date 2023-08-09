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

    std::unordered_set<entt::entity> entitiesToDestroy;

    for (auto bullet : playerBullets)
    {
        if (entitiesToDestroy.find(bullet) != entitiesToDestroy.end())
            continue;

        auto& bulletCollision = playerBullets.get<Collision>(bullet);

        enemies.each([&](auto enemy, auto& enemyCollision)
        {
            if (entitiesToDestroy.find(bullet) == entitiesToDestroy.end() &&
                entitiesToDestroy.find(enemy) == entitiesToDestroy.end() &&
                bulletCollision.collisionBox.intersects(enemyCollision.collisionBox))
            {
                entitiesToDestroy.insert(bullet);
                entitiesToDestroy.insert(enemy);

                //RESPAWN ONLY FOR TESTING
                EnemyInitializationSystem::createEnemy(registry, enemy2);
            }
        });
    }

    for (auto bullet : enemyBullets)
    {
        if (entitiesToDestroy.find(bullet) != entitiesToDestroy.end())
            continue;

        auto& bulletCollision = enemyBullets.get<Collision>(bullet);

        players.each([&](auto player, auto& playerCollision)
        {
            if (entitiesToDestroy.find(bullet) == entitiesToDestroy.end() &&
                entitiesToDestroy.find(player) == entitiesToDestroy.end() &&
                bulletCollision.collisionBox.intersects(playerCollision.collisionBox))
            {
                entitiesToDestroy.insert(bullet);
                //TODO: handle player death
            }
        });
    }

    for (auto entity : entitiesToDestroy)
        registry.destroy(entity);
}