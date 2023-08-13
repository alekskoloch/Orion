#include "DebugSystem.h"

#include "../components/components.h"

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
    