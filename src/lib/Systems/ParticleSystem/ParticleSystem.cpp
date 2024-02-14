#include "pch.h"
#include "ParticleSystem.h"

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