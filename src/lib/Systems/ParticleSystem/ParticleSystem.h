#pragma once

#include <entt/entt.hpp>

class ParticleSystem
{
public:
    ParticleSystem(entt::registry& registry);

private:
    entt::registry& registry;
};