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
    static void createBullet(entt::registry& registry, entt::entity& entity, sf::Vector2f targetPosition, bool right = false, float offset = 0.f)
    {
        auto weapon = registry.get<Weapon>(entity);
        auto position = registry.get<Position>(entity).position;

        sf::Vector2f direction(CalculateDirectionBetweenPoints(position, targetPosition, offset));
        float rotation = CalculateAzimuthAngleInDegrees(direction, 90.f);

        sf::Sprite sprite = CreateSprite(weapon.name + "_bullet");
        sprite.setRotation(rotation);

        auto bulletEntity = registry.create();
        registry.emplace<Bullet>(bulletEntity);
        registry.emplace<BulletOwnerTag>(bulletEntity);

        registry.emplace<Position>(bulletEntity, position);
        //TODO: Get Collision from weaponSchema
        registry.emplace<Collision>(bulletEntity, sprite.getGlobalBounds());

        switch (weapon.type)
        {
            case WeaponType::SingleShot:
            case WeaponType::TrippleShot:
                registry.emplace<Velocity>(bulletEntity, direction * weapon.bulletSpeed);
                break;
            case WeaponType::Shuriken:
            case WeaponType::DoubleShuriken:
                registry.emplace<Velocity>(bulletEntity, sf::Vector2f(0.f, 0.f));
                registry.emplace<ShurikenBullet>(bulletEntity, targetPosition, position, right, weapon.bulletSpeed);
                break;
            default:
                break;
        }

        registry.emplace<Renderable>(bulletEntity, sprite);
    }
};