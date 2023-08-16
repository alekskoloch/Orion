#include "DebugSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void DebugSystem::renderCollisionBoxes(entt::registry& registry, sf::RenderWindow& window)
{
    auto collisionView = registry.view<Collision>();
    for (auto entity : collisionView)
    {
        const auto& collision = collisionView.get<Collision>(entity);

        sf::RectangleShape rectBox;

        rectBox.setSize(sf::Vector2f(collision.collisionBox.width, collision.collisionBox.height));

        rectBox.setPosition(collision.collisionBox.left, collision.collisionBox.top);

        rectBox.setFillColor(sf::Color::Transparent);
        rectBox.setOutlineColor(sf::Color::White);
        rectBox.setOutlineThickness(-1.0f);

        window.draw(rectBox);
    }
}

void DebugSystem::renderAttackRangeCircles(entt::registry& registry, sf::RenderWindow& window)
{
    auto enemiesView = registry.view<Enemy, Position, EntityState>();
    for (auto entity : enemiesView)
    {
        //TODO: Make this configurable
        sf::CircleShape circle(1000.f);

        auto entityPosition = enemiesView.get<Position>(entity);

        circle.setPosition(entityPosition.position.x - 1000.f, entityPosition.position.y - 1000.f);

        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(-2.0f);

        window.draw(circle);
    }
}
    