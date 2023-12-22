#include "WeaponsSystem.h"

#include "../managers/TextureManager.h"

#include "../systems/PlayerInitializationSystem.h"
#include "../systems/SkillSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void WeaponsSystem::changeWeapon(entt::registry& registry, WeaponSchema weapon)
{
    auto player = registry.view<Player>();
    for (auto& entity : player)
    {
        registry.remove<Weapon>(entity);
        WeaponsSystem::loadWeapon(registry, weapon, entity);
    }
}

//TODO: Make template function for loading schema
void WeaponsSystem::loadWeapon(entt::registry& registry, const WeaponSchema& weaponSchema, entt::entity ownerEntity)
{
    TextureManager::getInstance().loadTexture(weaponSchema.bulletTextureName, ASSETS_PATH + weaponSchema.bulletTextureName + ".png");
    TextureManager::getInstance().loadTexture(weaponSchema.weaponIconTextureName, ASSETS_PATH + weaponSchema.weaponIconTextureName + ".png");

    if (weaponSchema.bulletTextureName == "red_bullet")
        loadWeaponFromConfig(registry, ownerEntity, Weapons::RedWeapon);
    else
        registry.emplace<Weapon>(ownerEntity, weaponSchema.weaponType, weaponSchema.damage, weaponSchema.cooldownTime, weaponSchema.bulletSpeed, weaponSchema.bulletTextureName, weaponSchema.weaponIconTextureName, weaponSchema.energyCost, weaponSchema.specialShotType, weaponSchema.energyCostForSpecialShot, weaponSchema.specialShotCooldownTime, weaponSchema.shoot, weaponSchema.specialShoot, weaponSchema.bulletsInSalvo, weaponSchema.queueCooldown);
}

void WeaponsSystem::loadWeaponFromConfig(entt::registry& registry, entt::entity ownerEntity, Weapons weaponID)
{
    std::ifstream configFile(CONFIG_PATH + std::string("weaponConfig.json"));
    if (!configFile.is_open())
        throw std::runtime_error("Could not open config file");
    
    nlohmann::json configJson;
    configFile >> configJson;

    if (!configJson.contains("weapons") || !configJson["weapons"].is_array())
        throw std::runtime_error("Could not find weapons array in config file");
    else
    {
        for (const auto& weapon : configJson["weapons"])
        {
            if (static_cast<int>(weaponID) == weapon["id"])
            {
                registry.emplace<Weapon>(
                    ownerEntity,
                    WeaponType::SingleShot,
                    weapon["damage"],
                    weapon["cooldown"],
                    weapon["bulletSpeed"],
                    "red_bullet",
                    "red_weapon",
                    weapon["shotCost"],
                    SpecialShotType::FullCircleShoot,
                    weapon["specialShotCost"],
                    5.f,
                    WeaponsSystem::getWeaponShotFunction(static_cast<ShotType>(weapon["shot"])),
                    WeaponsSystem::getWeaponShotFunction(static_cast<ShotType>(weapon["specialShot"])),
                    0,
                    0.f
                );
            }
        }
    }


    
}

void WeaponsSystem::updateWeaponCooldown(entt::registry& registry, sf::Time deltaTime)
{
    auto view = registry.view<Weapon>();
    for (auto& entity : view)
    {
        auto& weapon = view.get<Weapon>(entity);
        if (weapon.currentCooldownTime > 0.f)
        {
            weapon.currentCooldownTime -= deltaTime.asSeconds();
            if (weapon.currentCooldownTime < 0.f)
                weapon.currentCooldownTime = 0.f;
        }
    }
}

float WeaponsSystem::getWeaponDamage(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());
    
    auto& playerWeapon = registry.get<Weapon>(registry.view<Player>().front());

    float damage = playerWeapon.damage;
    float multiplier = 1.f;

    multiplier += playerSkillsComponent.damageMultiplier - 1.f;
    multiplier += playerSkillsComponent.weaponDamageMultiplier - 1.f;

    if (playerWeapon.weaponType == WeaponType::SingleShot)
    {
        multiplier += playerSkillsComponent.singleShotWeaponDamageMultiplier - 1.f;
    }
    else if (playerWeapon.weaponType == WeaponType::TrippleShot)
    {
        multiplier += playerSkillsComponent.tripleShotWeaponDamageMultiplier - 1.f;
    }

    return damage * multiplier;
}

float WeaponsSystem::getWeaponShotEnergyCost(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());

    auto& playerWeapon = registry.get<Weapon>(registry.view<Player>().front());

    float energyCost = playerWeapon.energyCost;
    float multiplier = 1.f;

    multiplier += playerSkillsComponent.weaponEnergyCostMultiplier - 1.f;

    if (playerWeapon.weaponType == WeaponType::SingleShot)
    {
        multiplier += playerSkillsComponent.singleShotWeaponEnergyCostMultiplier - 1.f;
    }
    else if (playerWeapon.weaponType == WeaponType::TrippleShot)
    {
        multiplier += playerSkillsComponent.tripleShotWeaponEnergyCostMultiplier - 1.f;
    }

    return energyCost * multiplier;
}

float WeaponsSystem::getWeaponSpecialShotEnergyCost(entt::registry& registry)
{
    auto& playerSkillsComponent = registry.get<Skills>(registry.view<Player>().front());

    auto& playerWeapon = registry.get<Weapon>(registry.view<Player>().front());

    float energyCost = playerWeapon.energyCostForSpecialShot;
    float multiplier = 1.f;

    multiplier += playerSkillsComponent.weaponEnergyCostMultiplier - 1.f;

    if (playerWeapon.weaponType == WeaponType::SingleShot)
    {
        multiplier += playerSkillsComponent.singleSpecialShotWeaponEnergyCostMultiplier - 1.f;
    }
    else if (playerWeapon.weaponType == WeaponType::TrippleShot)
    {
        multiplier += playerSkillsComponent.tripleSpecialShotWeaponEnergyCostMultiplier - 1.f;
    }

    return energyCost * multiplier;
}

ShotFunction WeaponsSystem::getWeaponShotFunction(ShotType shotType)
{
    switch (shotType)
    {
    case ShotType::None:
        throw std::runtime_error("None shot type is not allowed");
    case ShotType::SingleShot:
        return [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
        {
            if (registry.any_of<Player>(entity))
            {
                sf::Vector2f targetPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                BulletSystem::createBullet<PlayerBullet>(registry, entity, targetPosition);
            }
            else if (registry.any_of<Enemy>(entity))
            {
                auto playerView = registry.view<Player>();
                auto playerEntity = playerView.front();

                sf::Vector2f targetPosition = registry.get<Position>(playerEntity).position;

                BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition);
            }
            else
            {
                throw std::runtime_error("Wrong entity type for single shot");
            }
        };
    case ShotType::TripleShot:
        return [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
        {
            if (registry.any_of<Player>(entity))
            {
                sf::Vector2f targetPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                float angleOffset[] = { -10.f, 0.f, 10.f };

                for (auto offset : angleOffset)
                    BulletSystem::createBullet<PlayerBullet>(registry, entity, targetPosition, false, offset);
            }
            else if (registry.any_of<Enemy>(entity))
            {
                auto playerView = registry.view<Player>();
                auto playerEntity = playerView.front();

                sf::Vector2f targetPosition = registry.get<Position>(playerEntity).position;

                float angleOffset[] = { -10.f, 0.f, 10.f };

                for (auto offset : angleOffset)
                    BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition, false, offset);
            }
            else
            {
                throw std::runtime_error("Wrong entity type for triple shot");
            }
        };
    case ShotType::Shuriken:
        return [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
        {
            throw std::runtime_error("Shuriken shot type is not implemented");
        };
    case ShotType::DoubleShuriken:
        return [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
        {
            throw std::runtime_error("Double shuriken shot type is not implemented");
        };
    case ShotType::Nail:
        return [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
        {
            throw std::runtime_error("Nail shot type is not implemented");
        };
    case ShotType::FullCircle:
        return [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
        {
            if (registry.any_of<Player>(entity))
            {
                sf::Vector2f targetPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                float angleOffset[] = { 0.f, 45.f, 90.f, 135.f, 180.f, 225.f, 270.f, 315.f };

                for (auto offset : angleOffset)
                    BulletSystem::createBullet<PlayerBullet>(registry, entity, targetPosition, false, offset);
            }
            else if (registry.any_of<Enemy>(entity))
            {
                auto playerView = registry.view<Player>();
                auto playerEntity = playerView.front();

                sf::Vector2f targetPosition = registry.get<Position>(playerEntity).position;

                float angleOffset[] = { 0.f, 45.f, 90.f, 135.f, 180.f, 225.f, 270.f, 315.f };

                for (auto offset : angleOffset)
                    BulletSystem::createBullet<EnemyBullet>(registry, entity, targetPosition, false, offset);
            }
            else
            {
                throw std::runtime_error("Wrong entity type for full circle shot");
            }
        };
    case ShotType::TripleSalvo:
        return [](entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)
        {
            if (registry.any_of<Player>(entity))
            {
                auto& playerWeapon = registry.get<Weapon>(entity);

                playerWeapon.bulletsInQueue = 5;
                playerWeapon.queueCooldown = 0.03f;
            }
            else if (registry.any_of<Enemy>(entity))
            {
                throw std::runtime_error("Triple salvo shot type is not implemented for enemies");
            }
            else
            {
                throw std::runtime_error("Wrong entity type for triple salvo shot");
            }
        };
    default:
        throw std::runtime_error("Unknown shot type");
    }

    return nullptr;
}