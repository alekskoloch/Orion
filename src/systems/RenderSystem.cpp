#include "RenderSystem.h"

#include "renderable.h"
#include "position.h"

void RenderSystem::renderEntities(sf::RenderWindow& window, entt::registry& registry)
{
    auto view = registry.view<Renderable, Position>();
    for (auto entity : view)
    {
        auto [position, renderable] = view.get<Position, Renderable>(entity);
        renderable.sprite.setPosition(position.position);
        window.draw(renderable.sprite);
    }
}