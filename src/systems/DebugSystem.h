#pragma once

#include "../pch.h"

#include "../managers/BackgroundManager.h"

class DebugSystem
{
public:
    static void renderCollisionBoxes(entt::registry& registry, sf::RenderWindow& window);
    static void renderAttackRangeCircles(entt::registry& registry, sf::RenderWindow& window);
    static void renderBackgroundTilesInfo(entt::registry& registry, sf::RenderWindow& window, std::vector<BackgroundTile>& backgroundTiles);
};