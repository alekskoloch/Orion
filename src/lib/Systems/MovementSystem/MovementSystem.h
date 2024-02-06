#pragma once

#include <entt/entt.hpp>
#include <SFML/System.hpp>

#include "position.h"
#include "velocity.h"
#include "input.h"
#include "movementBoost.h"
#include "player.h"

#include "EnergySystem.h"

class MovementSystem
{
public:
    static void updateMovement(entt::registry& registry, sf::Time deltaTime);

private:
    static void updateMovementBoost(entt::registry& registry, sf::Time deltaTime);
};