#pragma once

#include "../../pch.h"

enum class CircleSegmentState
{
    Hidden,
    Hover,
    Active
};

class GUICircleSegment : public sf::Drawable
{
public:
    GUICircleSegment(sf::Vector2f center, float radius, float startAngle, float endAngle, float thickness, sf::Color color)
        : center(center), radius(radius), startAngle(startAngle), endAngle(endAngle), thickness(thickness), color1(color), color2(color)
    {
        this->createCircleSegment();
        this->color2.a = 0;
    }

    void update(float deltaTime);

    void setState(CircleSegmentState state) { this->state = state; }
    
private:
    sf::Vector2f center;
    float radius;
    float thickness;

    float startAngle;
    float currentAngle = 0.0f;
    float endAngle;

    std::vector<sf::Vertex> segmentVertices;
    sf::Color color1;
    sf::Color color2;

    CircleSegmentState state = CircleSegmentState::Hidden;
    bool active = false;

    void createCircleSegment();
    sf::Color interpolateColor(sf::Color color1, sf::Color color2, float t);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(segmentVertices.data(), segmentVertices.size(), sf::LinesStrip, states);
    }
};