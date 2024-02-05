#include "DebugSystem.h"

#include "position.h"
#include "collision.h"
#include "entityState.h"
#include "enemy.h"

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
        auto attackRange = enemiesView.get<EntityState>(entity).attackRange;
        auto idleRange = enemiesView.get<EntityState>(entity).idleRange;

        sf::CircleShape attackCircle(attackRange);
        sf::CircleShape idleCircle(idleRange);

        auto entityPosition = enemiesView.get<Position>(entity);

        attackCircle.setPosition(entityPosition.position.x - attackRange, entityPosition.position.y - attackRange);
        idleCircle.setPosition(entityPosition.position.x - idleRange, entityPosition.position.y - idleRange);

        attackCircle.setFillColor(sf::Color::Transparent);
        attackCircle.setOutlineColor(sf::Color::Red);
        attackCircle.setOutlineThickness(-2.0f);

        idleCircle.setFillColor(sf::Color::Transparent);
        idleCircle.setOutlineColor(sf::Color::Green);
        idleCircle.setOutlineThickness(-2.0f);

        window.draw(attackCircle);
        window.draw(idleCircle);
    }
}

void DebugSystem::renderBackgroundTilesFrame(entt::registry& registry, sf::RenderWindow& window, std::vector<BackgroundTile>& backgroundTiles)
{
    for (auto& backgroundTile : backgroundTiles)
    {
        auto& tile = backgroundTile.getTile();

        sf::RectangleShape rectBox;

        rectBox.setSize(sf::Vector2f(tile.getSize().x, tile.getSize().y));

        rectBox.setPosition(tile.getPosition().x, tile.getPosition().y);
        rectBox.setOrigin(tile.getOrigin().x, tile.getOrigin().y);

        rectBox.setFillColor(sf::Color::Transparent);
        rectBox.setOutlineColor(sf::Color::White);
        rectBox.setOutlineThickness(-1.0f);

        window.draw(rectBox);
    }
}