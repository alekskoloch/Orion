#include "pch.h"
#include "RotateTowardsMouseSystem.h"

void RotateTowardsMouseSystem::rotateTowardsMouse(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window)
{
    //TODO: Temporary solution for player camera
    CameraSystem::setPlayerCamera(registry, window);

    auto view = registry.view<RotationTowardsMouse, Position, Renderable>();

    for (auto entity : view)
    {
        auto& rotation = view.get<RotationTowardsMouse>(entity);
        auto& position = view.get<Position>(entity);
        auto& renderable = view.get<Renderable>(entity);

        if (rotation.enabled)
        {
            sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        
            float distanceToMouse = CalculateDistance(mousePosition, position.position);

            if (distanceToMouse > rotation.minimalActivationDistance)
            {
                float targetAngle = CalculateAzimuthAngleInDegrees(mousePosition - position.position, 90.f);
                float currentAngle = renderable.sprite.getRotation();
                float angleDifference = AngleDifference(targetAngle, currentAngle);

                float rotationAmount = angleDifference;
                if (rotationAmount > rotation.sensitivity * deltaTime.asSeconds())
                {
                    rotationAmount = rotation.sensitivity * deltaTime.asSeconds();
                }
                else if (rotationAmount < -rotation.sensitivity * deltaTime.asSeconds())
                {
                    rotationAmount = -rotation.sensitivity * deltaTime.asSeconds();
                }

                renderable.sprite.setRotation(currentAngle + rotationAmount);
            }
        }
    }

    CameraSystem::setDefaultCamera(window);
}