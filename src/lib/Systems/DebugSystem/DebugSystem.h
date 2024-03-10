#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "BackgroundManager.h"

#include "EnemyInitializationSystem.h"

#include "position.h"
#include "collision.h"
#include "entityState.h"
#include "enemy.h"

class DebugSystem
{
public:
    static void update(entt::registry& registry, sf::RenderWindow& window);

    static void renderCollisionBoxes(entt::registry& registry, sf::RenderWindow& window);
    static void renderAttackRangeCircles(entt::registry& registry, sf::RenderWindow& window);
    static void renderBackgroundTilesFrame(entt::registry& registry, sf::RenderWindow& window, std::vector<BackgroundTile>& backgroundTiles);

private:
    static void creatingEnemies(entt::registry& registry, sf::RenderWindow& window);
    static void spawnEnemyOnMousePosition(entt::registry& registry, sf::RenderWindow& window);
};