#pragma once

#include "../pch.h"

#include "../components/components.h"

inline float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

inline float CalculateDistance(const sf::Vector2f& point1, const sf::Vector2f& point2)
{
    return std::hypot(point1.x - point2.x, point1.y - point2.y);
}

inline float DistanceToMouse(const sf::Vector2f& mousePosition, const Position& position)
{
    return CalculateDistance(mousePosition, position.position);
}

inline float TargetAngle(const sf::Vector2f& mousePosition, const Position& position, float offset = 90.0f)
{
    return std::atan2(mousePosition.y - position.position.y, mousePosition.x - position.position.x) * (180 / M_PI) + offset;
}

inline float AngleDifference(float targetAngle, float currentAngle)
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

inline sf::Vector2f NormalizeVector(const sf::Vector2f& vector)
{
    float length = std::hypot(vector.x, vector.y);
    
    if (length != 0.f)
        return sf::Vector2f(vector.x / length, vector.y / length);
    else
        return vector;
}