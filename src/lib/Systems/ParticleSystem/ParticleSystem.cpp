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