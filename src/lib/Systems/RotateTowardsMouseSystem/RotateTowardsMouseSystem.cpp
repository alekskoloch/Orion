#include "RotateTowardsMouseSystem.h"

#include "pch.h"

#include <numbers>

#include "position.h"
#include "renderable.h"
#include "rotationTowardsMouse.h"

#include "InputData.hpp"

void RotateTowardsMouseSystem::rotateTowardsMouse( entt::registry& registry, const Mouse::MouseState& mouseState,
                                                   sf::Time deltaTime )
{
    constexpr auto fullCircle = 360.0F;
    constexpr auto halfCircle = 360.0F / 2;
    constexpr auto quarterCircle = 360.0F / 4;

    auto view = registry.view< RotationTowardsMouse, Position, Renderable >();

    for ( auto entity : view )
    {
        auto& rotation = view.get< RotationTowardsMouse >( entity );
        auto& position = view.get< Position >( entity );
        auto& renderable = view.get< Renderable >( entity );

        if ( rotation.enabled )
        {
            sf::Vector2f mousePos{ mouseState.worldPosition.x, mouseState.worldPosition.y };
            sf::Vector2f direction = mousePos - position.position;

            const auto angleRad = std::atan2( direction.y, direction.x );
            auto targetAngleDeg = angleRad * halfCircle / std::numbers::pi_v< float >;
            targetAngleDeg += quarterCircle;

            const auto currentAngleDeg = renderable.sprite.getRotation().asDegrees();
            auto diff = targetAngleDeg - currentAngleDeg;

            while ( diff <= -halfCircle )
            {
                diff += fullCircle;
            }

            while ( diff > halfCircle )
            {
                diff -= fullCircle;
            }

            const auto maxStep = rotation.sensitivity * deltaTime.asSeconds();
            diff = std::min( diff, maxStep );
            diff = std::max( diff, -maxStep );

            renderable.sprite.setRotation( sf::degrees( currentAngleDeg + diff ) );
        }
    }
}