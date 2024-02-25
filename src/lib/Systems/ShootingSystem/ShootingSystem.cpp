#include "pch.h"
#include "ShootingSystem.h"

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

void handleSpecialShoot(entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
{
    auto& weapon = registry.get<Weapon>(entity);

    weapon.specialShot(registry, window, entity);
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
                weapon.specialShot(registry, window, entity);
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
            weapon.shot(registry, window, entity);
            SoundManager::getInstance().playSound("Shot");
            //handleShoot<PlayerBullet>(registry, entity, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
            weapon.SetCooldown();
        }

        weapon.shootLastFrame = input.shoot;
    }
}

void handleEnemyShooting(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window)
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
                enemyWeapon.shot(registry, window, enemyEntity);
                SoundManager::getInstance().playSound("EnemyShot");
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
    ShootingSystem::handleQueue(registry, deltaTime, window);
    CameraSystem::setDefaultCamera(window);

    handleEnemyShooting(registry, deltaTime, window);
}

void ShootingSystem::handleQueue(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window)
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
                if (weapon.type == WeaponType::TrippleShot)
                {
                    float angleOffset[] = { -10.f, 0.f, 10.f };
                    for (auto offset : angleOffset)
                        BulletSystem::createBullet<PlayerBullet>(registry, playerEntity, window.mapPixelToCoords(sf::Mouse::getPosition(window)), false, offset);
                }
                else if (weapon.type == WeaponType::SingleShot)
                {
                    sf::Vector2f targetPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    float angleOffset[8];
                    for (int i = 0; i < 8; i++)
                        angleOffset[i] = i * 45.f + (weapon.bulletsInQueue * 10.f);

                    for (auto offset : angleOffset)
                        BulletSystem::createBullet<PlayerBullet>(registry, playerEntity, window.mapPixelToCoords(sf::Mouse::getPosition(window)), false, offset);
                }

                weapon.bulletsInQueue--;
            }
        }
    }
}
