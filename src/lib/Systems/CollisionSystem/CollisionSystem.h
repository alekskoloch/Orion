#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "EventManager.h"

#include "ExperienceSystem.h"
#include "ProceduralGenerationSystem.h"
#include "CooldownSystem.h"
#include "NotifySystem.h"
#include "EnemyInitializationSystem.h"
#include "DropSystem.h"
#include "WeaponsSystem.h"

#include "EnemySchema.h"

#include "shield.h"
#include "health.h"
#include "info.h"
#include "experience.h"
#include "stoneInventory.h"
#include "moneyInventory.h"
#include "enemy.h"
#include "player.h"
#include "dropItem.h"
#include "entityState.h"

class CollisionSystem
{
public:
    static void updateCollisionBoxes(entt::registry& registry);
    static void checkCollisions(entt::registry& registry, sf::RenderWindow& window);
};