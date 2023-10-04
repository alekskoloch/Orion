#include "ShootingSystem.h"

#include "../systems/CameraSystem.h"
#include "../systems/EnergySystem.h"
#include "../systems/SkillSystem.h"
#include "../systems/CooldownSystem.h"
#include "../systems/ProceduralGenerationSystem.h"
#include "../systems/ShieldSystem.h"
#include "../systems/BulletSystem.h"

#include "../managers/TextureManager.h"
#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../utils/MathOperations.h"
#include "../utils/GraphicsOperations.h"

template <typename BulletOwnerTag>
void handleShoot(entt::registry& registry, entt::entity& entity, sf::Vector2f targetPosition)
{
    auto weapon = registry.get<Weapon>(entity);
    //TODO: Make this configurable
    float angleOffset[] = { -10.f, 0.f, 10.f };
    switch (weapon.weaponType)
    {
    case WeaponType::SingleShot:
        BulletSystem::createBullet<BulletOwnerTag>(registry, entity, targetPosition);
        break;
    case WeaponType::TrippleShot:
        for (auto offset : angleOffset)
            BulletSystem::createBullet<BulletOwnerTag>(registry, entity, targetPosition, false, offset);
        break;
    case WeaponType::Shuriken:
        BulletSystem::createBullet<BulletOwnerTag>(registry, entity, targetPosition, ProceduralGenerationSystem::GetRandomNumber(0, 1) == 1);
        break;
    case WeaponType::DoubleShuriken:
        BulletSystem::createBullet<BulletOwnerTag>(registry, entity, targetPosition, false);
        BulletSystem::createBullet<BulletOwnerTag>(registry, entity, targetPosition, true);
        break;
    default:
        break;
    }
}

void handleSpecialShoot(entt::registry& registry, entt::entity& entity, sf::Vector2f targetPosition)
{
    auto weapon = registry.get<Weapon>(entity);

    if (weapon.specialShotType == SpecialShotType::None)
    {
        return;
    }
    else if (weapon.specialShotType == SpecialShotType::FullCircleShoot)
    {
        float angleOffset[] = {  0.f, 45.f, 90.f, 135.f, 180.f, 225.f, 270.f, 315.f };
        for (auto offset : angleOffset)
            BulletSystem::createBullet<PlayerBullet>(registry, entity, targetPosition, offset);
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
            canShoot = input.shoot && weapon.currentCooldownTime == 0.f && !input.specialShot;
        else
            canShoot = input.shoot && !weapon.shootLastFrame && !input.specialShot;

        bool canSpecialShoot = false;

        if (weapon.weaponType == WeaponType::SingleShot)
            canSpecialShoot = SkillSystem::isSingleShotWeaponSpecialShotEnabled(registry);
        else if (weapon.weaponType == WeaponType::TrippleShot)
            canSpecialShoot = SkillSystem::isTripleShotWeaponSpecialShotEnabled(registry);
        else
            canSpecialShoot = SkillSystem::isAllWeaponsSpecialShotEnabled(registry);
            
        if (input.specialShot && canSpecialShoot && EnergySystem::hasEnoughEnergy<Player>(registry, SkillSystem::getWeaponSpecialShotEnergyCost(registry, entity)))
            if (CooldownSystem::getCooldown(registry, entity, "specialShot") == 0.f)
            {
                EnergySystem::removeEnergy<Player>(registry, SkillSystem::getWeaponSpecialShotEnergyCost(registry, entity));
                CooldownSystem::setCooldown(registry, entity, "specialShot", weapon.specialShotCooldownTime);
                handleSpecialShoot(registry, entity, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                //TODO: Chance should be configurable
                if (ProceduralGenerationSystem::GetRandomNumber(1,15) == 1)
                    ShieldSystem::getShield(registry, basicShield);
            }

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
