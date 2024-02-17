#pragma once

#include <entt/entt.hpp>
#include <SFML/System.hpp>

#include "ProceduralGenerationSystem.h"

#include "GraphicsOperations.h"

#include "drop.h"
#include "renderable.h"
#include "position.h"
#include "velocity.h"
#include "collision.h"
#include "removable.h"
#include "enemy.h"
#include "dropItem.h"


class DropSystem
{
public:
    static void drop(entt::registry& registry, entt::entity& entity);
    static void updateDrop(entt::registry& registry, sf::Time deltaTime);
};