#include "pch.h"
#include "ParticleFactory.h"

#include "ParticleBuilder.h"

Particle ParticleFactory::createFlameParticle(const sf::Vector2f& position, const sf::Vector2f& entityVelocity)
{
    return ParticleBuilder()
        .addLifetime(0.25f)
        .addRadius(3.f)
        .addStartColor(sf::Color(255, 255, 0))
        .addEndColor(sf::Color(255, 0, 0))
        .addPosition(position)
        .addVelocity(entityVelocity * 0.8f)
        .build();
}