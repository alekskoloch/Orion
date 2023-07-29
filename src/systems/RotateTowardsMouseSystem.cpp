#include "RotateTowardsMouseSystem.h"

#include <cmath>

#include "../components/components.h"

#include <iostream>

void RotateTowardsMouseSystem::rotateTowardsMouse(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window)
{
    auto view = registry.view<RotationTowardsMouse, Position, Renderable>();

    for (auto entity : view)
    {
        auto& rotation = view.get<RotationTowardsMouse>(entity);
        auto& position = view.get<Position>(entity);
        auto& renderable = view.get<Renderable>(entity);

        if (rotation.enabled)
        {
            sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            float distanceToMouse = std::sqrt(std::pow(mousePosition.x - position.position.x, 2) + std::pow(mousePosition.y - position.position.y, 2));

            if (distanceToMouse > rotation.minimalActivationDistance)
            {
                float targetAngle = std::atan2(mousePosition.y - position.position.y, mousePosition.x - position.position.x) * (180 / 3.14159265f) + 90;
                float currentAngle = renderable.sprite.getRotation();
                float angleDifference = targetAngle - currentAngle;

                if (angleDifference > 180.0f)
                {
                    angleDifference -= 360.0f;
                }
                else if (angleDifference < -180.0f)
                {
                    angleDifference += 360.0f;
                }

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
}