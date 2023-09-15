#include "ShootingSystem.h"

#include "../systems/CameraSystem.h"
#include "../systems/EnergySystem.h"
#include "../systems/SkillSystem.h"

#include "../managers/TextureManager.h"
#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../utils/MathOperations.h"
#include "../utils/GraphicsOperations.h"

template <typename BulletOwnerTag>
void createBullet(entt::registry& registry, entt::entity& entity, sf::Vector2f targetPosition, float offset = 0.f)
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

        if (canShoot && EnergySystem::hasEnoughEnergy<Player>(registry, SkillSystem::getWeaponEnergyCost(registry, entity)))
        {
            EnergySystem::removeEnergy<Player>(registry, SkillSystem::getWeaponEnergyCost(registry, entity));
            handleShoot<PlayerBullet>(registry, entity, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
            weapon.SetCooldown();
        }

        weapon.shootLastFrame = input.shoot;
    }
}

void handleEnemyShooting(entt::registry& registry, sf::Time deltaTime)
{
    auto enemyView = registry.view<Enemy, Weapon, Position, EntityState>();
    auto playerPosition = registry.view<Player, Position>().get<Position>(registry.view<Player, Position>().front()).position;
    for (auto enemyEntity : enemyView)
    {
        auto& enemyState = enemyView.get<EntityState>(enemyEntity);
        if (enemyState.currentState == EntityState::State::Attacking)
        {
            auto& enemyWeapon = enemyView.get<Weapon>(enemyEntity);
            auto& enemyPosition = enemyView.get<Position>(enemyEntity);

            if (enemyWeapon.currentCooldownTime == 0.f)
            {
                handleShoot<EnemyBullet>(registry, enemyEntity, playerPosition);
                enemyWeapon.SetCooldown();
            }
        }
    }
}

void ShootingSystem::shoot(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window)
{
    //TODO: Temporary solution for player camera
    CameraSystem::setPlayerCamera(registry, window);
    handlePlayerShooting(registry, deltaTime, window);
    CameraSystem::setDefaultCamera(window);

    handleEnemyShooting(registry, deltaTime);
}
