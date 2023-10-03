#pragma once

#include "../pch.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../utils/MathOperations.h"
#include "../utils/GraphicsOperations.h"

class BulletSystem
{
public:
    static void updateShurikenBullet(entt::registry& registry, sf::Time deltaTime)
    {
        auto shurikenBulletsView = registry.view<ShurikenBullet, Position>();

        for (auto shurikenBulletEntity : shurikenBulletsView)
        {
            auto& shurikenBulletData = shurikenBulletsView.get<ShurikenBullet>(shurikenBulletEntity);
            auto& position = shurikenBulletsView.get<Position>(shurikenBulletEntity);
            
            int multiplier = shurikenBulletData.right ? 1 : -1;

            float x1 = shurikenBulletData.enemyPositionWhenShot.x;
            float y1 = shurikenBulletData.enemyPositionWhenShot.y;

            float x2 = shurikenBulletData.playerPositionWhenShot.x;
            float y2 = shurikenBulletData.playerPositionWhenShot.y;

            float distanceBetweenPoints = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
            float radius = distanceBetweenPoints / 2.f;

            float bulletX = ((x1 + x2) / 2) + std::cos((shurikenBulletData.currentTime * multiplier) * (shurikenBulletData.speed/distanceBetweenPoints) + std::atan2(y2 - ((y1 + y2) / 2), x2 - (x1 + x2) / 2)) * radius;
            float bulletY = ((y1 + y2) / 2) + std::sin((shurikenBulletData.currentTime * multiplier) * (shurikenBulletData.speed/distanceBetweenPoints) + std::atan2(y2 - ((y1 + y2) / 2), x2 - (x1 + x2) / 2)) * radius;

            position.position = {bulletX, bulletY};

            shurikenBulletData.currentTime += deltaTime.asSeconds();

            if ((shurikenBulletData.currentTime * shurikenBulletData.speed) / distanceBetweenPoints > 6.25)
                registry.destroy(shurikenBulletEntity);
        }
    }

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