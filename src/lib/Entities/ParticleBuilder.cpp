#include "pch.h"
#include "ParticleBuilder.h"

ParticleBuilder::ParticleBuilder()
{
    
}

ParticleBuilder& ParticleBuilder::addLifetime(float lifetime)
{
    particle.lifetime = lifetime;

    return *this;
}

ParticleBuilder& ParticleBuilder::addRadius(float radius)
{
    particle.particle.setRadius(radius);
    particle.particle.setOrigin(radius / 2.f, radius / 2.f);

    return *this;
}

ParticleBuilder& ParticleBuilder::addStartColor(sf::Color startColor)
{
    particle.particle.setFillColor(startColor);
    particle.startColor = startColor;

    return *this;
}

ParticleBuilder& ParticleBuilder::addEndColor(sf::Color endColor)
{
    particle.endColor = endColor;

    return *this;
}

ParticleBuilder& ParticleBuilder::addPosition(sf::Vector2f position)
{
    particle.particle.setPosition(position);

    return *this;
}

ParticleBuilder& ParticleBuilder::addVelocity(sf::Vector2f velocity)
{
    particle.velocity = velocity;

    return *this;
}

Particle ParticleBuilder::build()
{
    return particle;
}