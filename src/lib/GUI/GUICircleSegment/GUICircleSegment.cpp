#include "pch.h"
#include "GUICircleSegment.h"

void GUICircleSegment::update(float deltaTime)
{
    if (!this->active)
    {
        if (this->state == CircleSegmentState::Hover)
        {
            if (currentAngle < endAngle)
            {
                currentAngle += 1000.0f * deltaTime;
                if (currentAngle > endAngle)
                    currentAngle = endAngle;
            }

            this->createCircleSegment();
        }
        else
        {
            if (currentAngle > startAngle)
            {
                currentAngle -= 1000.0f * deltaTime;
                if (currentAngle < startAngle)
                    currentAngle = startAngle;
            }
            this->createCircleSegment();
        }

        if (this->state == CircleSegmentState::Active)
        {
            this->active = true;
            color2 = color1;
            currentAngle = endAngle;
            this->createCircleSegment();
        }
    }
}

sf::Color GUICircleSegment::interpolateColor(sf::Color color1, sf::Color color2, float t)
{
    sf::Color color;
    color.r = static_cast<sf::Uint8>((1 - t) * color1.r + t * color2.r);
    color.g = static_cast<sf::Uint8>((1 - t) * color1.g + t * color2.g);
    color.b = static_cast<sf::Uint8>((1 - t) * color1.b + t * color2.b);
    color.a = static_cast<sf::Uint8>((1 - t) * color1.a + t * color2.a);

    return color;
}

void GUICircleSegment::createCircleSegment()
{
    this->segmentVertices.clear();

    int numSegments = static_cast<int>(std::ceil((currentAngle - startAngle)));

    float angleStep = (currentAngle - startAngle) / numSegments;

    for (int i = 0; i < numSegments; i++)
    {
        float angle1 = startAngle + i * angleStep;
        float angle2 = startAngle + (i + 1) * angleStep;
        float t = static_cast<float>(i) / numSegments;

        sf::Color color = interpolateColor(color1, color2, t);

        float midPoint = (angle1 + angle2) / 2.0f;
        float alpha = std::abs(midPoint - currentAngle) / (currentAngle - startAngle);

        for (float t = angle1; t < angle2; t += 0.5f)
        {
            float angle = t * M_PI / 180.0f;
            sf::Vector2f outerPoint = center + sf::Vector2f(std::cos(angle), std::sin(angle)) * radius;
            sf::Vector2f innerPoint = center + sf::Vector2f(std::cos(angle), std::sin(angle)) * (radius - thickness);

            segmentVertices.emplace_back(outerPoint, color);
            segmentVertices.emplace_back(innerPoint, color);
        }
    }
}