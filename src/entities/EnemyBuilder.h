#pragma once

#include "../pch.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

class EnemyBuilder
{
public:
    EnemyBuilder(entt::registry& registry, entt::entity enemy);

private:
    entt::registry& registry;
    entt::entity enemy;
};