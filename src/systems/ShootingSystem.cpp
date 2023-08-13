#include "ShootingSystem.h"

#include "../managers/TextureManager.h"
#include "../components/components.h"
#include "../components/tagComponents.h"

template <typename BulletOwnerTag>
void createBullet(entt::registry& registry, entt::entity& entity, sf::Vector2f targetPosition, float offset = 0.f)
{
    auto weapon = registry.get<Weapon>(entity);
    auto position = registry.get<Position>(entity);

    sf::Vector2f direction = targetPosition - position.position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f normalizedDirection = direction / length;

    float angleInRadians = std::atan2(normalizedDirection.y, normalizedDirection.x);
    angleInRadians += offset * (M_PI / 180.f);
    sf::Vector2f newDirection(std::cos(angleInRadians), std::sin(angleInRadians));

    float rotation = std::atan2(newDirection.y, newDirection.x) * (180.f / M_PI) + 90.f;

    sf::Sprite sprite(TextureManager::getInstance().getTexture(weapon.bulletTextureName));
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    sprite.setRotation(rotation);

    auto bulletEntity = registry.create();
    registry.emplace<Bullet>(bulletEntity);
    registry.emplace<BulletOwnerTag>(bulletEntity);

    registry.emplace<Position>(bulletEntity, position.position);
    //TODO: Get Collision from weaponSchema
    registry.emplace<Collision>(bulletEntity, sprite.getGlobalBounds());

    registry.emplace<Velocity>(bulletEntity, newDirection * weapon.bulletSpeed);
    registry.emplace<Renderable>(bulletEntity, sprite);
}

template <typename BulletOwnerTag>
void handleShoot(entt::registry& registry, entt::entity& entity, sf::Vector2f targetPosition)
{
    auto weapon = registry.get<Weapon>(entity);
    //TODO: Make this configurable
    float angleOffset[] = { -10.f, 0.f, 10.f };
    switch (weapon.weaponType)
    {
    case WeaponType::SingleShot:
        createBullet<BulletOwnerTag>(registry, entity, targetPosition);
        break;
    case WeaponType::TrippleShot:
        for (auto offset : angleOffset)
            createBullet<BulletOwnerTag>(registry, entity, targetPosition, offset);
        break;
    default:
        break;
    }
}

void handlePlayerShooting(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window)
{
    auto view = registry.view<Weapon, Input>();
    for (auto entity : view)
    {
        auto& weapon = view.get<Weapon>(entity);
        auto& input = view.get<Input>(entity);

        bool canShoot = false;

        if (weapon.autofire)
            canShoot = input.shoot && weapon.currentCooldownTime == 0.f;
        else
            canShoot = input.shoot && !weapon.shootLastFrame;

        if (canShoot)
        {
            handleShoot<PlayerBullet>(registry, entity, sf::Vector2f(sf::Mouse::getPosition(window)));
            weapon.currentCooldownTime = weapon.cooldownTime;
        }

        weapon.shootLastFrame = input.shoot;
    }
}

void handleEnemyShooting(entt::registry& registry, sf::Time deltaTime)
{
    auto enemyView = registry.view<Enemy, Weapon, Position>();
    auto playerPosition = registry.view<Player, Position>().get<Position>(registry.view<Player, Position>().front()).position;
    for (auto enemyEntity : enemyView)
    {
        auto& enemyWeapon = enemyView.get<Weapon>(enemyEntity);
        auto& enemyPosition = enemyView.get<Position>(enemyEntity);

        if (enemyWeapon.currentCooldownTime == 0.f)
        {
            handleShoot<EnemyBullet>(registry, enemyEntity, playerPosition);
            enemyWeapon.currentCooldownTime = enemyWeapon.cooldownTime;
        }
    }
}

void ShootingSystem::shoot(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window)
{
    handlePlayerShooting(registry, deltaTime, window);
    handleEnemyShooting(registry, deltaTime);
}
