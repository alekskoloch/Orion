#include "ShootingSystem.h"

#include "ShootingSystem.h"

#include "../managers/TextureManager.h"
#include "../components/components.h"

#include <iostream>

void ShootingSystem::shoot(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Weapon, Input, Position, Renderable>();
    for (auto entity : view)
    {
        auto& weapon = view.get<Weapon>(entity);
        auto& input = view.get<Input>(entity);
        auto& playerPosition = view.get<Position>(entity);
        auto& playerRenderable = view.get<Renderable>(entity);

        bool canShoot = false;

        if (weapon.autofire)
        {
            canShoot = input.shoot && weapon.currentCooldownTime == 0.f;
        }
        else
        {
            canShoot = input.shoot && !weapon.shootLastFrame;
        }

        if (weapon.currentCooldownTime > 0.f)
        {
            weapon.currentCooldownTime -= deltaTime.asSeconds();
            if (weapon.currentCooldownTime < 0.f)
                weapon.currentCooldownTime = 0.f;
        }

        if (canShoot)
        {
            //TODO: Refactor this
            if (weapon.weaponType == WeaponType::SingleShot)
            {
                auto bulletEntity = registry.create();

                sf::Sprite sprite(TextureManager::getInstance().getTexture(weapon.bulletTextureName));
                sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);

                registry.emplace<Position>(bulletEntity, playerPosition.position);

                float rotation = playerRenderable.sprite.getRotation();
                sprite.setRotation(rotation);

                float adjustedRotation = 90.f - rotation;
                float radians = adjustedRotation * (3.14159265f / 180.f);
                sf::Vector2f velocity(std::cos(radians) * weapon.bulletSpeed, -std::sin(radians) * weapon.bulletSpeed);

                registry.emplace<Velocity>(bulletEntity, velocity);
                registry.emplace<Renderable>(bulletEntity, sprite);

                weapon.currentCooldownTime = weapon.cooldownTime;
            }
            else if (weapon.weaponType == WeaponType::TrippleShot)
            {
                float angleOffset[] = { -10.f, 0.f, 10.f };

                for (float offset : angleOffset)
                {
                    auto bulletEntity = registry.create();

                    sf::Sprite sprite(TextureManager::getInstance().getTexture(weapon.bulletTextureName));
                    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);

                    registry.emplace<Position>(bulletEntity, playerPosition.position);

                    float rotation = playerRenderable.sprite.getRotation() + offset; // Adding offset for triple shot
                    sprite.setRotation(rotation);

                    float adjustedRotation = 90.f - rotation;
                    float radians = adjustedRotation * (3.14159265f / 180.f);
                    sf::Vector2f velocity(std::cos(radians) * weapon.bulletSpeed, -std::sin(radians) * weapon.bulletSpeed);

                    registry.emplace<Velocity>(bulletEntity, velocity);
                    registry.emplace<Renderable>(bulletEntity, sprite);
                }

                weapon.currentCooldownTime = weapon.cooldownTime;
            }
        }

        weapon.shootLastFrame = input.shoot;
    }
}
