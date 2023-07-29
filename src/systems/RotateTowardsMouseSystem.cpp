#include "RotateTowardsMouseSystem.h"

#include <cmath>

#include "../components/components.h"

#include <iostream>

float DistanceToMouse(const sf::Vector2f& mousePosition, const Position& position)
{
    return std::sqrt(std::pow(mousePosition.x - position.position.x, 2) + std::pow(mousePosition.y - position.position.y, 2));
}

float TargetAngle(const sf::Vector2f& mousePosition, const Position& position, float offset = 90.0f)
{
    return std::atan2(mousePosition.y - position.position.y, mousePosition.x - position.position.x) * (180 / M_PI) + offset;
}

float AngleDifference(float targetAngle, float currentAngle)
{
    float angleDifference = targetAngle - currentAngle;
    if (angleDifference > 180.0f)
    {
        angleDifference -= 360.0f;
    }
    else if (angleDifference < -180.0f)
    {
        angleDifference += 360.0f;
    }
    return angleDifference;
}

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