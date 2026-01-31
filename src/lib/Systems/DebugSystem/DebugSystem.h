#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "BackgroundManager.h"

namespace Mouse
{
struct MouseState;
} // namespace Mouse

class Window;

class DebugSystem
{
public:
    static void update( entt::registry& registry, const Mouse::MouseState& mouseState );

    static void renderCollisionBoxes( entt::registry& registry, Window& window );
    static void renderAttackRangeCircles( entt::registry& registry, Window& window );
    static void renderBackgroundTilesFrame( entt::registry& registry, Window& window,
                                            std::vector< BackgroundTile >& backgroundTiles );

private:
    static void creatingEnemies( entt::registry& registry, const Mouse::MouseState& mouseState );
    static void spawnEnemyOnMousePosition( entt::registry& registry, const Mouse::MouseState& mouseState );
};