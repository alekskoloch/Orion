#include "pch.h"
#include "MovementSystem.h"

//TODO: This should be configurable

static sf::Vector2f boostMovementValue = sf::Vector2f(0.f, 0.f);

void MovementSystem::updateMovement(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Position, Velocity>();
    for (auto entity : view)
    {
        auto& position = view.get<Position>(entity);
        auto& velocity = view.get<Velocity>(entity);

        position.position += velocity.velocity * deltaTime.asSeconds();
    }

    updateMovementBoost(registry, deltaTime);
}

void MovementSystem::updateMovementBoost(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Position, Velocity, Input, MovementBoost>();
    for (auto entity : view)
    {
        auto& position = view.get<Position>(entity);
        auto& input = view.get<Input>(entity);
        auto& movementBoost = view.get<MovementBoost>(entity);

        if (input.readyForMovementBoost)
        {
            if (EnergySystem::hasEnoughEnergy<Player>(registry, 30.f))
            {
                boostMovementValue = sf::Vector2f(0.f, 0.f);

                if (input.up)
                    boostMovementValue.y -= movementBoost.boostValue;
                if (input.down)
                    boostMovementValue.y += movementBoost.boostValue;

                if (input.left)
                    boostMovementValue.x -= movementBoost.boostValue;
                if (input.right)
                    boostMovementValue.x += movementBoost.boostValue;

                if (!input.up && !input.down && !input.left && !input.right)
                {
                    auto& velocity = view.get<Velocity>(entity);
                    if (velocity.velocity.x > 0)
                        boostMovementValue.x += movementBoost.boostValue;
                    else if (velocity.velocity.x < 0)
                        boostMovementValue.x -= movementBoost.boostValue;

                    if (velocity.velocity.y > 0)
                        boostMovementValue.y += movementBoost.boostValue;
                    else if (velocity.velocity.y < 0)
                        boostMovementValue.y -= movementBoost.boostValue;
                }

                if (boostMovementValue.x != 0.f || boostMovementValue.y != 0.f)
                {
                    EnergySystem::removeEnergy<Player>(registry, 30.f);
                    SoundManager::getInstance().playSound("MovementBoost");
                }

                input.readyForMovementBoost = false;
            }
        }

        position.position += boostMovementValue * deltaTime.asSeconds();

        if (boostMovementValue.x != 0.f)
        {
            if (boostMovementValue.x > 0)
            {
                boostMovementValue.x -= movementBoost.boostDecelerationValue * deltaTime.asSeconds();
                if (boostMovementValue.x < 0)
                    boostMovementValue.x = 0.f;
            }
            else if (boostMovementValue.x < 0)
            {
                boostMovementValue.x += movementBoost.boostDecelerationValue * deltaTime.asSeconds();
                if (boostMovementValue.x > 0)
                    boostMovementValue.x = 0.f;
            }
        }

        if (boostMovementValue.y != 0.f)
        {
            if (boostMovementValue.y > 0)
            {
                boostMovementValue.y -= movementBoost.boostDecelerationValue * deltaTime.asSeconds();
                if (boostMovementValue.y < 0)
                    boostMovementValue.y = 0.f;
            }
            else if (boostMovementValue.y < 0)
            {
                boostMovementValue.y += movementBoost.boostDecelerationValue * deltaTime.asSeconds();
                if (boostMovementValue.y > 0)
                    boostMovementValue.y = 0.f;
            }
        }
    }
}