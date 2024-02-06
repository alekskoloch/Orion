#pragma once

#include <entt/entt.hpp>

#include "SoundManager.h"

#include "health.h"

class HealthSystem
{
public:
    static void updateHealth(entt::registry& registry);
};