#pragma once

#include "../pch.h"

class EnergySystem
{
public:
    static void updateEnergy(entt::registry& registry, sf::Time deltaTime);
};