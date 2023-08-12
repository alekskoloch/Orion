#include "ShootingSystem.h"

#include "../managers/TextureManager.h"
#include "../components/components.h"
#include "../components/tagComponents.h"

template <typename BulletOwnerTag>
void createBullet(entt::registry& registry, sf::Vector2f position, sf::FloatRect& hitbox, sf::Sprite& sprite, sf::Vector2f velocity)
{
    auto bulletEntity = registry.create();
    registry.emplace<Bullet>(bulletEntity);
    registry.emplace<BulletOwnerTag>(bulletEntity);

    registry.emplace<Position>(bulletEntity, position);
    registry.emplace<Collision>(bulletEntity, hitbox);

    registry.emplace<Velocity>(bulletEntity, velocity);
    registry.emplace<Renderable>(bulletEntity, sprite);
}

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
                sf::Sprite sprite(TextureManager::getInstance().getTexture(weapon.bulletTextureName));
                sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);

                sf::FloatRect collisionRect(playerPosition.position.x - 3.5f, playerPosition.position.y - 3.5f, 7.f, 7.f);

                float rotation = playerRenderable.sprite.getRotation();
                sprite.setRotation(rotation);
                float adjustedRotation = 90.f - rotation;
                float radians = adjustedRotation * (3.14159265f / 180.f);

                sf::Vector2f velocity(std::cos(radians) * weapon.bulletSpeed, -std::sin(radians) * weapon.bulletSpeed);

                createBullet<PlayerBullet>(registry, playerPosition.position, collisionRect, sprite, velocity);

                weapon.currentCooldownTime = weapon.cooldownTime;
            }
            else if (weapon.weaponType == WeaponType::TrippleShot)
            {
                float angleOffset[] = { -10.f, 0.f, 10.f };

                for (float offset : angleOffset)
                {
                    sf::Sprite sprite(TextureManager::getInstance().getTexture(weapon.bulletTextureName));
                    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);

                    sf::FloatRect collisionRect(playerPosition.position.x - 3.5f, playerPosition.position.y - 3.5f, 7.f, 7.f);

                    float rotation = playerRenderable.sprite.getRotation() + offset;
                    sprite.setRotation(rotation);
                    float adjustedRotation = 90.f - rotation;
                    float radians = adjustedRotation * (3.14159265f / 180.f);
                    sf::Vector2f velocity(std::cos(radians) * weapon.bulletSpeed, -std::sin(radians) * weapon.bulletSpeed);

                    createBullet<PlayerBullet>(registry, playerPosition.position, collisionRect, sprite, velocity);
                }

                weapon.currentCooldownTime = weapon.cooldownTime;
            }
        }

        weapon.shootLastFrame = input.shoot;
    }

    auto enemyView = registry.view<Enemy, Weapon, Position, Renderable>();
    auto playerView = registry.view<Player, Position>();
    for (auto enemyEntity : enemyView)
    {
        auto& enemyWeapon = enemyView.get<Weapon>(enemyEntity);
        auto& enemyPosition = enemyView.get<Position>(enemyEntity);

        for (auto playerEntity : playerView)
        {
            auto& playerPosition = playerView.get<Position>(playerEntity);

            if (enemyWeapon.currentCooldownTime > 0.f)
            {
                enemyWeapon.currentCooldownTime -= deltaTime.asSeconds();
                if (enemyWeapon.currentCooldownTime < 0.f)
                    enemyWeapon.currentCooldownTime = 0.f;
            }

            if (enemyWeapon.currentCooldownTime == 0.f)
            {
                sf::Vector2f direction = playerPosition.position - enemyPosition.position;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                sf::Vector2f normalizedDirection = direction / length;

                // auto bulletEntity = registry.create();
                // registry.emplace<Bullet>(bulletEntity);
                // registry.emplace<EnemyBullet>(bulletEntity);

                sf::Sprite sprite(TextureManager::getInstance().getTexture(enemyWeapon.bulletTextureName));
                sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);

                sf::FloatRect collisionRect(sprite.getGlobalBounds());

                float rotation = std::atan2(normalizedDirection.y, normalizedDirection.x) * (180.f / 3.14159265f) + 90.f;
                sprite.setRotation(rotation);

                // registry.emplace<Position>(bulletEntity, enemyPosition.position);
                // registry.emplace<Collision>(bulletEntity, sprite.getGlobalBounds());

                sf::Vector2f velocity = normalizedDirection * enemyWeapon.bulletSpeed;

                // registry.emplace<Velocity>(bulletEntity, velocity);
                // registry.emplace<Renderable>(bulletEntity, sprite);

                createBullet<EnemyBullet>(registry, enemyPosition.position, collisionRect, sprite, velocity);

                enemyWeapon.currentCooldownTime = enemyWeapon.cooldownTime;
            }
        }
    }
}
