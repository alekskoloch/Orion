#pragma once

#include "particle.h"

class ParticleFactory
{
public:
    Particle createFlameParticle(const sf::Vector2f& position, const sf::Vector2f& entityVelocity);
    Particle createExplosionParticle(const sf::Vector2f& position, const sf::Color& color);
};