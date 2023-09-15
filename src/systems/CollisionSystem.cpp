#include "CollisionSystem.h"

#include "EnemyInitializationSystem.h"
#include "DropSystem.h"

#include "../systems/SkillSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../schema/EnemySchema.h"

template <typename BulletOwnerTag, typename TargetTag>
void checkBulletCollitions(entt::registry& registry, std::unordered_set<entt::entity>& entitiesToDestroy)
{
    auto bullets = registry.view<Collision, BulletOwnerTag>();
    auto targets = registry.view<Collision, TargetTag>();

    for (auto bullet : bullets)
    {
        if (entitiesToDestroy.find(bullet) != entitiesToDestroy.end())
            continue;

        targets.each([&](auto target, auto& targetCollision)
        {
            if (entitiesToDestroy.find(bullet) == entitiesToDestroy.end() &&
                entitiesToDestroy.find(target) == entitiesToDestroy.end() &&
                bullets.template get<Collision>(bullet).collisionBox.intersects(targetCollision.collisionBox))
            {
                entitiesToDestroy.insert(bullet);
                //TODO: make onCollision method for each tag
                if constexpr (std::is_same_v<TargetTag, Player>)
                {
                    auto shieldPlayerView = registry.view<Player, Shield>();

                    for (auto shield : shieldPlayerView)
                    {
                        auto isActive = shieldPlayerView.get<Shield>(shield).active;

                        if (isActive)
                        {
                            auto& shieldDuration = shieldPlayerView.get<Shield>(shield).currentDuration;
                            shieldDuration = 0;
                        }
                        else
                        {
                            //TODO: Temporary effect
                            auto playerEnergyView = registry.view<Player, Energy>();
                            for (auto player : playerEnergyView)
                            {
                                auto& playerEnergy = playerEnergyView.get<Energy>(player);
                                playerEnergy.currentEnergyValue = 0.f;
                            }
                        }
                    }
                }
                else if constexpr (std::is_same_v<TargetTag, Enemy>)
                {
                    //entitiesToDestroy.insert(target);

                    auto& enemyHealthComponent = registry.get<Health>(target);
                    auto player = registry.view<Player>().front();
                    enemyHealthComponent.currentHealthValue -= SkillSystem::getWeaponDamage(registry, player);

                    //TODO: Handle it in a separate system
                    if (auto* dropComponent = registry.try_get<Drop>(target))
                        DropSystem::drop(registry, target);

                    //TODO: Only for testing
                    auto thisEnemy = registry.get<Name>(target);

                    if (thisEnemy.name == "enemy")
                        EnemyInitializationSystem::createEnemy(registry, enemy);
                    else if (thisEnemy.name == "enemy2")
                        EnemyInitializationSystem::createEnemy(registry, enemy2);
                    else if (thisEnemy.name == "enemy3")
                        EnemyInitializationSystem::createEnemy(registry, enemy3);
                    else if (thisEnemy.name == "enemy4")
                        EnemyInitializationSystem::createEnemy(registry, enemy4);
                }
            }
        });
    }
}

void checkDropCollision(entt::registry& registry)
{
    auto drops = registry.view<Collision, DropItem>();
    auto players = registry.view<Collision, Player>();

    for (auto drop : drops)
    {
        for (auto player : players)
        {
            auto& dropCollision = drops.get<Collision>(drop);
            auto& playerCollision = players.get<Collision>(player);

            if (dropCollision.collisionBox.intersects(playerCollision.collisionBox))
            {
                registry.destroy(drop);
                //TODO: Only temporary effect
                auto& playerEnergy = registry.get<Energy>(player);
                playerEnergy.currentEnergyValue = playerEnergy.maxEnergyValue;
            }
        }
    }
}

void destroyEntities(entt::registry& registry, std::unordered_set<entt::entity>& entitiesToDestroy)
{
    for (auto entity : entitiesToDestroy)
        registry.destroy(entity);
}

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
    std::unordered_set<entt::entity> entitiesToDestroy;

    checkBulletCollitions<PlayerBullet, Enemy>(registry, entitiesToDestroy);
    checkBulletCollitions<EnemyBullet, Player>(registry, entitiesToDestroy);

    checkDropCollision(registry);
    
    destroyEntities(registry, entitiesToDestroy);
}