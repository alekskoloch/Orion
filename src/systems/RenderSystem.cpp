#include "RenderSystem.h"

#include "../components/components.h"

void RenderSystem::renderEntities(sf::RenderWindow& window, entt::registry& registry)
{
    auto view = registry.view<Renderable, Position>();
    for (auto entity : view)
    {
        auto [position, renderable] = view.get<Position, Renderable>(entity);
        renderable.sprite.setPosition(position.position);
        window.draw(renderable.sprite);
    }

    //TODO: Make this a separate debug system
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