#include "MovementSystem.h"

#include "../components/components.h"

//TODO: This should be configurable

void MovementSystem::updateMovement(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Position, Input, Speed>();
    for (auto entity : view)
    {
        auto& position = view.get<Position>(entity);
        auto& input = view.get<Input>(entity);
        auto& speed = view.get<Speed>(entity);

        sf::Vector2f direction(0.f, 0.f);

        if (input.up)
            direction.y -= 1.f;
        if (input.down)
            direction.y += 1.f;;
        if (input.left)
            direction.x -= 1.f;
        if (input.right)
            direction.x += 1.f;

        //TODO: Velocity
        position.position += direction * speed.speed * deltaTime.asSeconds();
    }
}