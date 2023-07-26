#include "MovementSystem.h"

#include "../components/components.h"

//TODO: This should be configurable

void MovementSystem::updateMovement(entt::registry& registry)
{
    auto view = registry.view<Position, Input>();
    for (auto entity : view)
    {
        auto& position = view.get<Position>(entity);
        auto& input = view.get<Input>(entity);

        sf::Vector2f direction(0.f, 0.f);

        if (input.up)
            direction.y -= 1.f;
        if (input.down)
            direction.y += 1.f;;
        if (input.left)
            direction.x -= 1.f;
        if (input.right)
            direction.x += 1.f;

        //TODO: Speed should be component
        //TODO: Velocity
        position.position += direction * 10.f;
    }
}