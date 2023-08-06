#pragma once

#include "../pch.h"

class AccelerationSystem
{
public:
    static void accelerate(entt::registry& registry, sf::Time deltaTime);
};