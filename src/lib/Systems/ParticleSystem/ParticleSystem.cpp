#include "pch.h"
#include "ParticleSystem.h"

#include "player.h"
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

void ParticleSystem::createFlameEffect(const sf::Vector2f &position, const sf::Vector2f& entityVelocity)
{
    sf::Vector2f normalizedVelocity = entityVelocity / std::sqrt(entityVelocity.x * entityVelocity.x + entityVelocity.y * entityVelocity.y);

    unsigned int particleCount;

    //TODO: Refactor this
    if (std::abs(entityVelocity.x) < 200 && std::abs(entityVelocity.y) < 200)
    {
        particleCount = 3;
    }
    else if (std::abs(entityVelocity.x) < 600 && std::abs(entityVelocity.y) < 600)
    {
        particleCount = 5;
    }
    else
    {
        particleCount = 7;
    }

    for (unsigned int i = 0; i < particleCount; i++)
    {
        auto particle = std::make_unique<Particle>();
        particle->particle.setRadius(particleRadius);
        particle->particle.setOrigin(particleRadius / 2.f, particleRadius / 2.f);
        particle->particle.setFillColor(startColor);

        sf::Vector2f offset = normalizedVelocity * (-particleRadius * i);

        particle->particle.setPosition(position + offset);
        particle->velocity = entityVelocity * 0.8f;
        particle->lifetime = lifetime;

        particles.push_back(std::move(particle));
    }
}