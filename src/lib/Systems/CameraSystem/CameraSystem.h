#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

class CameraSystem
{
public:
    static void initializeZoom();
    static void zoomFactorChange( float delta );
    static void updateZoomFactor( sf::Time deltaTime );
    static void updateCamera( sf::View& view, entt::registry& registry );

private:
    static float zoomFactor;
    static float zoomFactorTarget;
};