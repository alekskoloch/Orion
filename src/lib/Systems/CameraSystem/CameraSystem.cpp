#include "pch.h"
#include "CameraSystem.h"

void CameraSystem::setDefaultCamera(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
}

void CameraSystem::setPlayerCamera(entt::registry& registry, sf::RenderWindow& window)
{
    auto view = registry.view<Player, Position>();
    for (auto entity : view)
    {
        auto& playerPosition = view.get<Position>(entity);

        sf::View view = window.getView();
        view.setCenter(playerPosition.position);
        window.setView(view);
    }
}

void CameraSystem::setZoomFactor(float zoomFactor, sf::RenderWindow& window)
{
    sf::View view = window.getView();
    view.zoom(zoomFactor);
    window.setView(view);
}