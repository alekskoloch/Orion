#include "pch.h"
#include "ParticleSystem.h"

#include "player.h"
#include "enemy.h"
#include "removable.h"
#include "position.h"
#include "velocity.h"
#include "renderable.h"
#include "input.h"

#include "MathOperations.h"

//TODO: Only temprorary variables
const float lifetime = 0.25f;
const float particleRadius = 3.f;
const sf::Color startColor(255, 255, 0);
const sf::Color endColor(255, 0, 0);

ParticleSystem::ParticleSystem(entt::registry &registry) : registry(registry)
{

}

void ParticleSystem::draw(sf::RenderWindow &window)
{
    for (auto &particle : particles)
    {
        window.draw(particle->particle);
    }
}

void ParticleSystem::removeDeadParticles()
{
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const auto& particle) { return particle->lifetime <= 0; }),
        particles.end());
}

void ParticleSystem::adjustParticlesVisibility()
{
    for (auto &particle : particles)
    {
        if (particle->lifetime < lifetime)
        {            
            float interpolation = particle->lifetime / lifetime;
            sf::Color interpolatedColor(
                static_cast<sf::Uint8>(startColor.r + (endColor.r - startColor.r) * (1 - interpolation)),
                static_cast<sf::Uint8>(startColor.g + (endColor.g - startColor.g) * (1 - interpolation)),
                static_cast<sf::Uint8>(startColor.b + (endColor.b - startColor.b) * (1 - interpolation)),
                static_cast<sf::Uint8>(255 * interpolation)
            );

            particle->particle.setFillColor(interpolatedColor);
        }
    }
}

void ParticleSystem::createFlameEffect(const sf::Vector2f &position, const sf::Vector2f& entityVelocity)
{
    sf::Vector2f normalizedVelocity = entityVelocity / std::sqrt(entityVelocity.x * entityVelocity.x + entityVelocity.y * entityVelocity.y);

    unsigned int particleCount;

    //TODO: Refactor this
    if (std::abs(entityVelocity.x) < 200 && std::abs(entityVelocity.y) < 200)
        particleCount = 3;
    else if (std::abs(entityVelocity.x) < 600 && std::abs(entityVelocity.y) < 600)
        particleCount = 5;
    else
        particleCount = 7;

    ParticleFactory particleFactory;

    for (unsigned int i = 0; i < particleCount; i++)
        particles.push_back(std::make_unique<Particle>(particleFactory.createFlameParticle(position + normalizedVelocity * (-3.f * i), entityVelocity)));
}

void ParticleSystem::createExplosionEffect(const sf::Vector2f &position)
{
    ParticleFactory particleFactory;

    //TODO: Particle number should be configurable
    for (unsigned int i = 0; i < 60; i++)
        particles.push_back(std::make_unique<Particle>(particleFactory.createExplosionParticle(position, sf::Color(255, 255, 0))));
}