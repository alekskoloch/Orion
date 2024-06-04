#include "pch.h"
#include "AreaGuardSystem.hpp"

#include "player.h"
#include "position.h"
#include "velocity.h"
#include "acceleration.h"

AreaGuardSystem::AreaGuardSystem(entt::registry& registry)
    : registry(registry)
{
    this->initializeAreaRectangle();
}

void AreaGuardSystem::initializeAreaRectangle()
{
    this->areaRectangle.setFillColor(sf::Color::Transparent);
    this->areaRectangle.setOutlineColor(sf::Color::Red);
    this->areaRectangle.setOutlineThickness(3.f);
}

void AreaGuardSystem::setGuardArea(sf::FloatRect guardArea)
{
    this->guardArea = guardArea;

    this->areaRectangle.setSize(sf::Vector2f(this->guardArea.width, this->guardArea.height));
    this->areaRectangle.setPosition(this->guardArea.left, this->guardArea.top);

    this->active = true;
}

void AreaGuardSystem::disableGuardArea()
{
    this->active = false;
}

void AreaGuardSystem::update(sf::Time deltaTime)
{
    if (this->active)
    {
        auto playerView = registry.view<Player, Position, Velocity, Acceleration>();

        auto& playerPosition = playerView.get<Position>(playerView.front());

        if (!this->guardArea.contains(playerPosition.position))
        {
            auto& playerVelocity = playerView.get<Velocity>(playerView.front());
            auto& playerAcceleration = playerView.get<Acceleration>(playerView.front());

            sf::Vector2f direction = {this->guardArea.left + this->guardArea.width / 2 - playerPosition.position.x,
                                      this->guardArea.top + this->guardArea.height / 2 - playerPosition.position.y};

            playerVelocity.velocity = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y) * playerAcceleration.accelerationValue;
        }

        const float distance = 1000.f;

        if (playerPosition.position.x < this->guardArea.left + distance ||
            playerPosition.position.x > this->guardArea.left + this->guardArea.width - distance ||
            playerPosition.position.y < this->guardArea.top + distance ||
            playerPosition.position.y > this->guardArea.top + this->guardArea.height - distance)
        {
            float alpha = 0.0f;

            if(!this->guardArea.contains(playerPosition.position))
            {
                alpha = 1.f;
            }
            else
            {
                if (playerPosition.position.x < this->guardArea.left + distance)
                {
                    float diff = this->guardArea.left + distance - playerPosition.position.x;
                    alpha = std::max(alpha, diff / distance);
                }
                if (playerPosition.position.x > this->guardArea.left + this->guardArea.width - distance)
                {
                    float diff = playerPosition.position.x - (this->guardArea.left + this->guardArea.width - distance);
                    alpha = std::max(alpha, diff / distance);
                }
                if (playerPosition.position.y < this->guardArea.top + distance)
                {
                    float diff = this->guardArea.top + distance - playerPosition.position.y;
                    alpha = std::max(alpha, diff / distance);
                }
                if (playerPosition.position.y > this->guardArea.top + this->guardArea.height - distance)
                {
                    float diff = playerPosition.position.y - (this->guardArea.top + this->guardArea.height - distance);
                    alpha = std::max(alpha, diff / distance);
                }
            }

            this->areaRectangle.setOutlineColor(sf::Color(255, 0, 0, 255 * alpha));
        }
        else
        {
            this->areaRectangle.setOutlineColor(sf::Color::Transparent);
        }
    }
}

void AreaGuardSystem::draw(sf::RenderWindow& window)
{
    if (this->active)
    {
        window.draw(this->areaRectangle);
    }
}