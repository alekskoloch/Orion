#pragma once

#include "particle.h"

class ParticleBuilder
{
public:
    ParticleBuilder();

    ParticleBuilder& addLifetime(float lifetime);
    ParticleBuilder& addRadius(float radius);
    ParticleBuilder& addStartColor(sf::Color startColor);
    ParticleBuilder& addEndColor(sf::Color endColor);
    ParticleBuilder& addPosition(sf::Vector2f position);
    ParticleBuilder& addVelocity(sf::Vector2f velocity);
    
    Particle build();

private:
    Particle particle;
};