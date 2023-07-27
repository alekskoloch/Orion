#include "AccelerationSystem.h"

#include "../components/components.h"

void AccelerationSystem::accelerate(entt::registry& registry)
{
    auto view = registry.view<Acceleration, Input, Speed, Velocity>();

    for (auto entity : view)
    {
        auto& acceleration = view.get<Acceleration>(entity);
        auto& input = view.get<Input>(entity);
        auto& speed = view.get<Speed>(entity);
        auto& velocity = view.get<Velocity>(entity);

        if (input.up)
        {
            if (velocity.velocity.y > -speed.speed)
                velocity.velocity.y -= acceleration.accelerationValue;
            else
                velocity.velocity.y = -speed.speed;
        }
        else if (velocity.velocity.y < 0)
        {
            velocity.velocity.y += acceleration.decelerationValue;
        }

        if (input.down)
        {
            if (velocity.velocity.y < speed.speed)
                velocity.velocity.y += acceleration.accelerationValue;
            else
                velocity.velocity.y = speed.speed;
        }
        else if (velocity.velocity.y > 0)
        {
            velocity.velocity.y -= acceleration.decelerationValue;
        }

        if (input.left)
        {
            if (velocity.velocity.x > -speed.speed)
                velocity.velocity.x -= acceleration.accelerationValue;
            else
                velocity.velocity.x = -speed.speed;
        }
        else if (velocity.velocity.x < 0)
        {
            velocity.velocity.x += acceleration.decelerationValue;
        }

        if (input.right)
        {
            if (velocity.velocity.x < speed.speed)
                velocity.velocity.x += acceleration.accelerationValue;
            else
                velocity.velocity.x = speed.speed;
        }
        else if (velocity.velocity.x > 0)
        {
            velocity.velocity.x -= acceleration.decelerationValue;
        }
    }
}