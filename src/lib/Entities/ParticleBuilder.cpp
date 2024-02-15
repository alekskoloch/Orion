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
}

ParticleBuilder& ParticleBuilder::addPosition(sf::Vector2f position)
{
    particle.particle.setPosition(position);
}

ParticleBuilder& ParticleBuilder::addVelocity(sf::Vector2f velocity)
{
    particle.velocity = velocity;
}

Particle ParticleBuilder::build()
{
    return particle;
}