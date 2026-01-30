#include "pch.h"
#include "ShootingSystem.h"

#include "CameraSystem.h"
#include "EnergySystem.h"
#include "SkillSystem.h"
#include "WeaponsSystem.h"
#include "CooldownSystem.h"
#include "ProceduralGenerationSystem.h"
#include "ShieldSystem.h"
#include "BulletSystem.h"

#include "TextureManager.h"

#include "player.h"
#include "enemy.h"
#include "weapon.h"
#include "input.h"
#include "position.h"
#include "entityState.h"


#include "MathOperations.h"
#include "GraphicsOperations.h"

template <typename BulletOwnerTag>
void handleShoot(entt::registry& registry, entt::entity& entity, sf::Vector2f targetPosition)
{
    auto weapon = registry.get<Weapon>(entity);
    //TODO: Make this configurable
    float angleOffset[] = { -10.f, 0.f, 10.f };
    switch (weapon.type)
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

void handleSpecialShoot(entt::registry& registry, const Mouse::MouseState& mouseState, entt::entity& entity)
{
    auto& weapon = registry.get<Weapon>(entity);

    weapon.specialShot(registry, mouseState, entity);
}

void handlePlayerShooting(entt::registry& registry, sf::Time deltaTime, const Mouse::MouseState& mouseState)
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

        if (weapon.type == WeaponType::SingleShot)
            canSpecialShoot = SkillSystem::isSkillEnabled(registry, SkillType::SingleShotWeaponSpecialShot);
        else if (weapon.type == WeaponType::TrippleShot)
            canSpecialShoot = SkillSystem::isSkillEnabled(registry, SkillType::TripleShotWeaponSpecialShot);
        else
            canSpecialShoot = SkillSystem::isSkillEnabled(registry, SkillType::AllWeaponsSpecialShot);
            
        if (input.specialShot && canSpecialShoot && EnergySystem::hasEnoughEnergy<Player>(registry, WeaponsSystem::getWeaponSpecialShotEnergyCost(registry)))
            if (CooldownSystem::getCooldown(registry, entity, "specialShot") == 0.f)
            {
                SoundManager::getInstance().playSound("SpecialShot");
                EnergySystem::removeEnergy<Player>(registry, WeaponsSystem::getWeaponSpecialShotEnergyCost(registry));
                CooldownSystem::setCooldown(registry, entity, "specialShot", weapon.specialShotCooldown);
                weapon.specialShot(registry, mouseState, entity);
                //handleSpecialShoot(registry, entity, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                //TODO: Chance should be configurable
                if (SkillSystem::isSkillEnabled(registry, SkillType::ShieldChanceForSingleSpecialShot) || SkillSystem::isSkillEnabled(registry, SkillType::ShieldChanceForTripleSpecialShot))
                    if (weapon.type == WeaponType::SingleShot)
                        ShieldSystem::getShield(registry, basicShield);
                    else if (weapon.type == WeaponType::TrippleShot)
                        ShieldSystem::getShield(registry, advancedShield);
            }

        if (canShoot && EnergySystem::hasEnoughEnergy<Player>(registry, WeaponsSystem::getWeaponShotEnergyCost(registry)))
        {
            EnergySystem::removeEnergy<Player>(registry, WeaponsSystem::getWeaponShotEnergyCost(registry));
            weapon.shot(registry, mouseState, entity);
            SoundManager::getInstance().playSound("Shot");
            //handleShoot<PlayerBullet>(registry, entity, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
            weapon.SetCooldown();
        }

        weapon.shootLastFrame = input.shoot;
    }
}

void handleEnemyShooting(entt::registry& registry, sf::Time deltaTime, const Mouse::MouseState& mouseState)
{
    auto enemyView = registry.view<Enemy, Weapon, Position, EntityState>();
    auto playerPosition = registry.view<Player, Position>().get<Position>(registry.view<Player>().front()).position;

    for (auto enemyEntity : enemyView)
    {
        auto& enemyState = enemyView.get<EntityState>(enemyEntity);

        if (enemyState.stateMachine->state_cast<const Attacking*>() != nullptr)
        {
            auto& enemyWeapon = enemyView.get<Weapon>(enemyEntity);
            auto& enemyPosition = enemyView.get<Position>(enemyEntity);

            if (enemyWeapon.currentCooldownTime <= 0.f)
            {
                enemyWeapon.shot(registry, mouseState, enemyEntity);
                SoundManager::getInstance().playSound("EnemyShot");
                enemyWeapon.SetCooldown();
            }
        }
    }
}

void ShootingSystem::shoot(entt::registry& registry, sf::Time deltaTime, const Mouse::MouseState& mouseState)
{
    //TODO: Temporary solution for player camera
    handlePlayerShooting(registry, deltaTime, mouseState);
    ShootingSystem::handleQueue(registry, deltaTime, mouseState);

    handleEnemyShooting(registry, deltaTime, mouseState);
}

void ShootingSystem::handleQueue(entt::registry& registry, sf::Time deltaTime, const Mouse::MouseState& mouseState)
{
    auto playerView = registry.view<Player, Weapon>();
    for (auto playerEntity : playerView)
    {
        auto& weapon = playerView.get<Weapon>(playerEntity);
        if (weapon.bulletsInQueue > 0)
        {
            weapon.queueCooldownTime += deltaTime.asSeconds();
            if (weapon.queueCooldownTime >= weapon.queueCooldown)
            {
                weapon.queueCooldownTime = 0.f;

                //TODO: Temporary solution, should support queue for each type of weapon
                const sf::Vector2f targetPosition{ mouseState.worldPosition.x, mouseState.worldPosition.y };
                if (weapon.type == WeaponType::TrippleShot)
                {
                    const auto angleOffset = { -10.F, 0.F, 10.F };

                    for (auto offset : angleOffset)
                    {
                        BulletSystem::createBullet<PlayerBullet>(registry, playerEntity, targetPosition, false, offset);
                    }
                }
                else if ( weapon.type == WeaponType::SingleShot )
                {
                    const sf::Vector2f targetPosition{ mouseState.worldPosition.x, mouseState.worldPosition.y };

                    constexpr auto fullCircle{ 360.F };
                    constexpr auto numberOfBullets{ 8 };
                    constexpr auto offset{ fullCircle / numberOfBullets };
                    constexpr auto additionalOffset{ 10.0F };

                    std::vector< float > angleOffset;
                    angleOffset.resize( numberOfBullets );
                    for ( int i = 0; i < numberOfBullets; i++ )
                    {
                        angleOffset[ i ] = static_cast< float >( i ) * offset +
                                           ( static_cast< float >( weapon.bulletsInQueue ) * additionalOffset );
                    }

                    for ( auto offset : angleOffset )
                    {
                        BulletSystem::createBullet< PlayerBullet >( registry, playerEntity, targetPosition, false, offset );
                    }
                }

                weapon.bulletsInQueue--;
            }
        }
    }
}
