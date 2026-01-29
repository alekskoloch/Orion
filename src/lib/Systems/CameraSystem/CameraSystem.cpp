#include "CameraSystem.h"
#include "pch.h"

void CameraSystem::updateCamera( sf::View& view, entt::registry& registry, float zoomFactor )
{
    sf::Vector2f targetPosition = { 0.f, 0.f };
    auto playerView = registry.view< Player, Position >();

    for ( auto entity : playerView )
    {
        targetPosition = playerView.get< Position >( entity ).position;
        break;
    }

    view.setCenter( targetPosition );

    view.zoom( zoomFactor );
}