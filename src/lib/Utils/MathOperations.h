#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

inline float LinearInterpolation(float startValue, float endValue, float fraction)
{
    return startValue + fraction * (endValue - startValue);
}

inline sf::Vector2f NormalizeVector(const sf::Vector2f& vector)
{
    float length = std::hypot(vector.x, vector.y);
    
    if (length != 0.f)
        return sf::Vector2f(vector.x / length, vector.y / length);
    else
        return vector;
}

inline sf::Vector2f RotateVector(const sf::Vector2f& vector, float angleDegrees)
{
    float rad = angleDegrees * (M_PI / 180.0f);
    return sf::Vector2f(
        vector.x * cos(rad) - vector.y * sin(rad),
        vector.x * sin(rad) + vector.y * cos(rad)
    );
}

inline float CalculateDistance(const sf::Vector2f& point1, const sf::Vector2f& point2)
{
    return std::hypot(point1.x - point2.x, point1.y - point2.y);
}

inline double CalculateAzimuthAngleInRadians(const sf::Vector2f vector, float offsetInDegrees = 0.0f)
{
    sf::Vector2f normalizedVector = NormalizeVector(vector);
    return std::atan2(normalizedVector.y, normalizedVector.x) + offsetInDegrees * (M_PI / 180.f);
}

inline double CalculateAzimuthAngleInDegrees(const sf::Vector2f vector, float offsetInDegrees = 0.0f)
{
    return CalculateAzimuthAngleInRadians(vector, offsetInDegrees) * (180.f / M_PI);
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

inline sf::Vector2f CalculateVectorFromAngle(float angleInRadians)
{
    return sf::Vector2f(std::cos(angleInRadians), std::sin(angleInRadians));
}

inline sf::Vector2f CalculateDirectionBetweenPoints(const sf::Vector2f& startPoint, const sf::Vector2f& targetPoint, float offset = 0.0f)
{
    sf::Vector2f direction = NormalizeVector(targetPoint - startPoint);
    float angleInRadians = CalculateAzimuthAngleInRadians(direction, offset);
    return CalculateVectorFromAngle(angleInRadians);
}