#pragma once

#include "../pch.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../utils/MathOperations.h"
#include "../utils/GraphicsOperations.h"

class BulletSystem
{
public:
    template <typename BulletOwnerTag>
    static void createBullet(entt::registry& registry, entt::entity& entity, sf::Vector2f targetPosition, float offset = 0.f)
    {
        auto weapon = registry.get<Weapon>(entity);
        auto position = registry.get<Position>(entity).position;

        sf::Vector2f direction(CalculateDirectionBetweenPoints(position, targetPosition, offset));
        float rotation = CalculateAzimuthAngleInDegrees(direction, 90.f);

        sf::Sprite sprite = CreateSprite(weapon.bulletTextureName);
        sprite.setRotation(rotation);

        auto bulletEntity = registry.create();
        registry.emplace<Bullet>(bulletEntity);
        registry.emplace<BulletOwnerTag>(bulletEntity);

        registry.emplace<Position>(bulletEntity, position);
        //TODO: Get Collision from weaponSchema
        registry.emplace<Collision>(bulletEntity, sprite.getGlobalBounds());
        registry.emplace<Velocity>(bulletEntity, direction * weapon.bulletSpeed);
        registry.emplace<Renderable>(bulletEntity, sprite);
    }
};