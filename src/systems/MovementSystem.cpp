#include "MovementSystem.h"

#include "../components/components.h"

#include "../systems/EnergySystem.h"

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
    auto view = registry.view<Position, Input, MovementBoost>();
    for (auto entity : view)
    {
        auto& position = view.get<Position>(entity);
        auto& input = view.get<Input>(entity);
        auto& movementBoost = view.get<MovementBoost>(entity);

        if (input.readyForMovementBoost && EnergySystem::hasEnoughEnergy<Player>(registry, 30.f))
        {
            if (input.up || input.down || input.left || input.right)
            {
                boostMovementValue = sf::Vector2f(0.f, 0.f);

                input.movementBoostActive = true;
                if (input.up && !input.down && !input.left && !input.right)
                {
                    boostMovementValue.y -= movementBoost.boostValue;
                    EnergySystem::removeEnergy<Player>(registry, 30.f);
                }
                else if (input.down && !input.up && !input.left && !input.right)
                {
                    boostMovementValue.y += movementBoost.boostValue;
                    EnergySystem::removeEnergy<Player>(registry, 30.f);
                }
                else if (input.left && !input.right && !input.up && !input.down)
                {
                    boostMovementValue.x -= movementBoost.boostValue;
                    EnergySystem::removeEnergy<Player>(registry, 30.f);
                }
                else if (input.right && !input.left && !input.up && !input.down)
                {
                    boostMovementValue.x += movementBoost.boostValue;
                    EnergySystem::removeEnergy<Player>(registry, 30.f);
                }
                else if (input.up && !input.down && input.left && !input.right)
                {
                    boostMovementValue.y -= movementBoost.boostDecelerationValue;
                    boostMovementValue.x -= movementBoost.boostDecelerationValue;
                    EnergySystem::removeEnergy<Player>(registry, 30.f);
                }
                else if (input.up && !input.down && !input.left && input.right)
                {
                    boostMovementValue.y -= movementBoost.boostDecelerationValue;
                    boostMovementValue.x += movementBoost.boostDecelerationValue;
                    EnergySystem::removeEnergy<Player>(registry, 30.f);
                }
                else if (!input.up && input.down && input.left && !input.right)
                {
                    boostMovementValue.y += movementBoost.boostDecelerationValue;
                    boostMovementValue.x -= movementBoost.boostDecelerationValue;
                    EnergySystem::removeEnergy<Player>(registry, 30.f);
                }
                else if (!input.up && input.down && !input.left && input.right)
                {
                    boostMovementValue.y += movementBoost.boostDecelerationValue;
                    boostMovementValue.x += movementBoost.boostDecelerationValue;
                    EnergySystem::removeEnergy<Player>(registry, 30.f);
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