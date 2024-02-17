#pragma once

#include <entt/entt.hpp>

#include "particle.h"

#include "../Entities/ParticleFactory.h"

class ParticleSystem
{
public:
    ParticleSystem(entt::registry& registry);

    void draw(sf::RenderWindow &window);
private:
    entt::registry& registry;
    std::vector<std::unique_ptr<Particle>> particles;

    void handlePlayerMovementParticles();
    void handleEnemyExplosionParticles();

    void adjustParticlesVisibility();
    void createFlameEffect(const sf::Vector2f& position, const sf::Vector2f& entityVelocity);
    void createExplosionEffect(const sf::Vector2f& position);
    void removeDeadParticles();
};