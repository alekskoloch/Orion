#pragma once

#include "../pch.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../utils/MathOperations.h"
#include "../utils/GraphicsOperations.h"

class BulletSystem
{
public:
    static void updateShurikenBullet(entt::registry& registry, sf::Time deltaTime);

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

    template <typename BulletOwnerTag>
    static void createShurikenBullet(entt::registry& registry, entt::entity& entity, sf::Vector2f targetPosition, bool right)
    {
        auto weapon = registry.get<Weapon>(entity);
        auto position = registry.get<Position>(entity).position;

        sf::Vector2f direction(CalculateDirectionBetweenPoints(position, targetPosition));
        float rotation = CalculateAzimuthAngleInDegrees(direction, 90.f);

        sf::Sprite sprite = CreateSprite(weapon.bulletTextureName);
        sprite.setRotation(rotation);

        auto bulletEntity = registry.create();
        registry.emplace<Bullet>(bulletEntity);
        registry.emplace<BulletOwnerTag>(bulletEntity);
        registry.emplace<ShurikenBullet>(bulletEntity, targetPosition, position, right, weapon.bulletSpeed);

        registry.emplace<Position>(bulletEntity, position);
        registry.emplace<Collision>(bulletEntity, sprite.getGlobalBounds());
        registry.emplace<Velocity>(bulletEntity, sf::Vector2f(0.f, 0.f));
        registry.emplace<Renderable>(bulletEntity, sprite);
    }
};