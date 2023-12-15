#include "EnemyBuilder.h"

EnemyBuilder::EnemyBuilder(entt::registry& registry, entt::entity enemy)
    : registry(registry), enemy(enemy)
{
    registry.emplace<Enemy>(enemy);
    registry.emplace<EntityState>(enemy);
}