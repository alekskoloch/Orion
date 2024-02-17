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

Particle ParticleFactory::createExplosionParticle(const sf::Vector2f& position, const sf::Color& color)
{
    //TODO: Make this configurable
    const float minParticleRadius = 3.f;
    const float maxParticleRadius = 6.f;
    const float maxSpeed = 500.0f;
    const float minLifetime = 0.25f;
    const float maxLifetime = 2.f;

    //TODO: This is only a temporary solution for (255, 255, 0) color (yellow)
    sf::Color shadeOfColor = sf::Color(
        color.r - (std::rand() % 20),
        color.g - (std::rand() % 20),
        color.b + (std::rand() % 20)
    );

    return ParticleBuilder()
        .addLifetime(std::rand() % 100 / 100.f * (maxLifetime - minLifetime) + minLifetime)
        .addRadius(static_cast<float>(std::rand() % static_cast<int>(maxParticleRadius - minParticleRadius)) + minParticleRadius)
        .addStartColor(shadeOfColor)
        .addEndColor(sf::Color(shadeOfColor.r / 2, shadeOfColor.g / 2, shadeOfColor.b / 2))
        .addPosition(position)
        .addVelocity(sf::Vector2f(
            (std::rand() % 100 - 50) / 100.f * maxSpeed,
            (std::rand() % 100 - 50) / 100.f * maxSpeed
        ))
        .build();
}