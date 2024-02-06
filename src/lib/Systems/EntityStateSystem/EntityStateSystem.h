#pragma once

#include <entt/entt.hpp>
#include <SFML/System.hpp>

#include "enemy.h"
#include "player.h"
#include "position.h"
#include "entityState.h"

#include "MathOperations.h"

class EntityStateSystem
{
public:
    static void updateEntityState(entt::registry& registry);
};