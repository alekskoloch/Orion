#include "MovementSystem.h"

#include "../components/components.h"

//TODO: This should be configurable

void MovementSystem::updateMovement(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Position, Velocity>();
    for (auto entity : view)
    {
        auto& position = view.get<Position>(entity);
        auto& velocity = view.get<Velocity>(entity);

        position.position += velocity.velocity * deltaTime.asSeconds();
    }
}