#include "RotateTowardsMouseSystem.h"

#include "../utils/MathOperations.h"
#include "../components/components.h"

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
            float distanceToMouse = DistanceToMouse(mousePosition, position);

            if (distanceToMouse > rotation.minimalActivationDistance)
            {
                float targetAngle = TargetAngle(mousePosition, position);
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
}