#include "AccelerationSystem.h"

#include "acceleration.h"
#include "input.h"
#include "speed.h"
#include "velocity.h"

void AccelerationSystem::accelerate(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Acceleration, Input, Speed, Velocity>();

    for (auto entity : view)
    {
        auto& acceleration = view.get<Acceleration>(entity);
        auto& input = view.get<Input>(entity);
        auto& speed = view.get<Speed>(entity);
        auto& velocity = view.get<Velocity>(entity);

        auto updateVelocity = [&](float& axisVelocity, bool positiveDirection, bool inputDirection)
        {
            if (inputDirection)
            {
                if (positiveDirection ? axisVelocity < speed.maxSpeedValue : axisVelocity > -speed.maxSpeedValue)
                    axisVelocity += (positiveDirection ? 1 : -1) * acceleration.accelerationValue * deltaTime.asSeconds();
                else
                    axisVelocity = (positiveDirection ? 1 : -1) * speed.maxSpeedValue;
            }
            else if ((positiveDirection && axisVelocity > 0) || (!positiveDirection && axisVelocity < 0))
            {
                axisVelocity += (positiveDirection ? -1 : 1) * acceleration.decelerationValue * deltaTime.asSeconds();
                if ((positiveDirection && axisVelocity < 0) || (!positiveDirection && axisVelocity > 0))
                    axisVelocity = 0.0f;
            }
        };

        updateVelocity(velocity.velocity.y, false, input.up);
        updateVelocity(velocity.velocity.y, true, input.down);
        updateVelocity(velocity.velocity.x, false, input.left);
        updateVelocity(velocity.velocity.x, true, input.right);
    }
}