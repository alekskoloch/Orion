#include "CameraSystem.h"

#include <algorithm>
#include "pch.h"

#include "position.h"
#include "player.h"

#include "../Managers/ConfigManager/ConfigManager.hpp"

float CameraSystem::zoomFactor = 1.0F;
float CameraSystem::zoomFactorTarget = 1.0F;

void CameraSystem::initializeZoom()
{
    zoomFactor = 1 / ConfigManager::getInstance().getScale();
    zoomFactorTarget = zoomFactor;
}

void CameraSystem::zoomFactorChange( float delta )
{
    constexpr auto maxScaleFactor{ 2.0F };
    constexpr auto step{ 0.05F };

    const float scale = 1.0F / ConfigManager::getInstance().getScale();

    if ( delta > 0 && zoomFactorTarget > scale )
    {
        zoomFactorTarget -= step * scale;
        zoomFactorTarget = std::max(zoomFactorTarget, scale);
    }
    else if ( delta < 0 && zoomFactorTarget < scale * maxScaleFactor )
    {
        zoomFactorTarget += step * scale;
        zoomFactorTarget = std::min(zoomFactorTarget, scale * maxScaleFactor );
    }
}

void CameraSystem::updateZoomFactor( sf::Time deltaTime )
{
    constexpr auto zoomFactorSpeed = 5.0F;

    if ( zoomFactor != zoomFactorTarget )
    {
        float zoomFactorDelta = zoomFactorTarget - zoomFactor;
        float zoomFactorChange = zoomFactorDelta * deltaTime.asSeconds() * zoomFactorSpeed;
        zoomFactor += zoomFactorChange;
    }
}

void CameraSystem::updateCamera( sf::View& view, entt::registry& registry )
{
    sf::Vector2f targetPosition = { 0.f, 0.f };
    auto playerView = registry.view< Player, Position >();

    for ( auto entity : playerView )
    {
        targetPosition = playerView.get< Position >( entity ).position;
        break;
    }
    view.setCenter( targetPosition );

    const auto& config = ConfigManager::getInstance();
    sf::Vector2f baseSize( static_cast< float >( config.getScreenWidth() ), static_cast< float >( config.getScreenHeight() ) );

    view.setSize( baseSize * zoomFactor );
}