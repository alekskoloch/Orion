#include "PointSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../managers/TextureManager.h"

#include "../utils/MathOperations.h"

void PointSystem::addPointOfInterest(entt::registry& registry, sf::Vector2f pointPosition, std::string id)
{
    auto pointOfInterest = registry.create();
    registry.emplace<PointOfInterest>(pointOfInterest, id);
    registry.emplace<Position>(pointOfInterest, pointPosition);
    sf::Sprite sprite;
    sprite.setPosition(pointPosition);
    sprite.setTexture(TextureManager::getInstance().getTexture("pointOfInterest"));
    registry.emplace<Renderable>(pointOfInterest, sprite);
}

void PointSystem::removePointOfInterest(entt::registry& registry, sf::Vector2f pointPosition)
{
    auto view = registry.view<PointOfInterest, Position>();
    for (auto entity : view)
    {
        auto& position = view.get<Position>(entity);
        if (position.position == pointPosition)
        {
            registry.destroy(entity);
            return;
        }
    }
}

bool PointSystem::isPointOfInterestActive(entt::registry& registry, std::string id)
{
    auto view = registry.view<PointOfInterest>();
    for (auto entity : view)
    {
        auto& pointOfInterest = view.get<PointOfInterest>(entity);
        if (pointOfInterest.id == id)
            return true;
    }
    return false;
}

void PointSystem::update(entt::registry& registry, sf::Time deltaTime)
{    
    auto view = registry.view<PointOfInterest, Position>();
    for (auto entity : view)
    {
        auto& position = view.get<Position>(entity);

        auto playerView = registry.view<Player, Position>();
        for (auto playerEntity : playerView)
        {
            auto& playerPosition = playerView.get<Position>(playerEntity);
            if (CalculateDistance(playerPosition.position, position.position) < 100.f) //TODO: Make this configurable
            {
                registry.destroy(entity);
                return;
            }
        }
    }
}