#include "CollisionSystem.h"

#include <format>

#include "EnemyInitializationSystem.h"
#include "DropSystem.h"

#include "../managers/EventManager.h"

#include "../systems/WeaponsSystem.h"
#include "../systems/CooldownSystem.h"
#include "../systems/ExperienceSystem.h"
#include "../systems/ProceduralGenerationSystem.h"
#include "../systems/NotifySystem.h"

#include "../schema/EnemySchema.h"

template <typename BulletOwnerTag, typename TargetTag>
void checkBulletCollitions(entt::registry& registry, std::unordered_set<entt::entity>& entitiesToDestroy, sf::RenderWindow& window)
{
    auto bullets = registry.view<Collision, BulletOwnerTag>();
    auto targets = registry.view<Collision, TargetTag>();

    for (auto bullet : bullets)
    {
        if (entitiesToDestroy.find(bullet) != entitiesToDestroy.end())
            continue;

        targets.each([&](auto target, auto& targetCollision)
        {
            if (entitiesToDestroy.find(bullet) == entitiesToDestroy.end() &&
                entitiesToDestroy.find(target) == entitiesToDestroy.end() &&
                bullets.template get<Collision>(bullet).collisionBox.intersects(targetCollision.collisionBox))
            {
                //TODO: make onCollision method for each tag
                if constexpr (std::is_same_v<TargetTag, Player>)
                {
                    entitiesToDestroy.insert(bullet);

                    auto shieldPlayerView = registry.view<Player, Shield>();

                    for (auto shield : shieldPlayerView)
                    {
                        if (shieldPlayerView.get<Shield>(shield).active)
                        {
                            CooldownSystem::setCooldown(registry, registry.view<Player>().front(), "shieldCooldown", 0.f);
                        }
                        else
                        {
                            NotifySystem::notifyDialogBox(
                                window,
                                "You have been defeated. However, you can continue the game.",
                                "Ok",
                                []() {}
                            );
                            SoundManager::getInstance().playSound("Death");
                        }
                    }
                }
                else if constexpr (std::is_same_v<TargetTag, Enemy>)
                {

                    auto& enemyHealthComponent = registry.get<Health>(target);
                    if (enemyHealthComponent.currentHealthValue <= 0.f)
                        return;

                    entitiesToDestroy.insert(bullet);
                    auto player = registry.view<Player>().front();
                    enemyHealthComponent.currentHealthValue -= WeaponsSystem::getWeaponDamage(registry);
                    auto damageInfo = registry.create();

                    //TODO: Refactor this!
                    double damage = WeaponsSystem::getWeaponDamage(registry);

                    if (damage > 0.01)
                    {
                        std::string damageString;
                        
                        if (damage - static_cast<int>(damage) == 0.0)
                        {
                            damageString = std::format("{}", static_cast<int>(damage));
                        }
                        else
                        {
                            damageString = std::format("{:.2f}", damage);
                            if (damageString.back() == '0')
                            {
                                damageString.pop_back();
                                if (damageString.back() == '0')
                                {
                                    damageString.pop_back();
                                }
                            }
                        }

                        sf::Vector2f infoPosition;
                        infoPosition.x = registry.get<Position>(target).position.x + ProceduralGenerationSystem::GetRandomNumber(-50.f, 50.f);
                        infoPosition.y = registry.get<Position>(target).position.y + ProceduralGenerationSystem::GetRandomNumber(-50.f, 50.f);
                        
                        registry.emplace<Info>(damageInfo, damageString, infoPosition, sf::Color::Red);
                        SoundManager::getInstance().playSound("Hit");
                    }

                    if (enemyHealthComponent.currentHealthValue <= 0.f)
                    {
                        EventManager::getInstance().trigger(EventManager::Event::EnemyKilled);

                        auto enemyExpValue = registry.get<Experience>(target).experience;
                        ExperienceSystem::addExp(registry, enemyExpValue);

                        sf::Vector2f infoPosition;
                        infoPosition.x = registry.get<Position>(target).position.x + 50.f;
                        infoPosition.y = registry.get<Position>(target).position.y - 50.f;

                        auto expInfo = registry.create();
                        //TODO: Randomize and animate info position
                        registry.emplace<Info>(expInfo, std::to_string(enemyExpValue), infoPosition, sf::Color::Yellow);
                    }
                }
            }
        });
    }
}

void checkDropCollision(entt::registry& registry)
{
    auto drops = registry.view<Collision, DropItem>();
    auto players = registry.view<Collision, Player>();

    for (auto drop : drops)
    {
        for (auto player : players)
        {
            auto& dropCollision = drops.get<Collision>(drop);
            auto& playerCollision = players.get<Collision>(player);

            if (dropCollision.collisionBox.intersects(playerCollision.collisionBox))
            {
                auto dropName = registry.get<DropItem>(drop).name;

                if (dropName == "stoneGreen")
                {
                    auto& playerStones = registry.get<StoneInventory>(player);
                    playerStones.greenStones++;
                    registry.destroy(drop);
                }
                else if (dropName == "stoneOrange")
                {
                    auto& playerStones = registry.get<StoneInventory>(player);
                    playerStones.orangeStones++;
                    registry.destroy(drop);
                }
                else if (dropName == "money10")
                {
                    auto& playerMoney = registry.get<MoneyInventory>(player);
                    playerMoney.money += 10;
                    registry.destroy(drop);
                }
                else if (dropName == "money20")
                {
                    auto& playerMoney = registry.get<MoneyInventory>(player);
                    playerMoney.money += 20;
                    registry.destroy(drop);
                }
                else if (dropName == "money50")
                {
                    auto& playerMoney = registry.get<MoneyInventory>(player);
                    playerMoney.money += 50;
                    registry.destroy(drop);
                }
                else if (dropName == "money100")
                {
                    auto& playerMoney = registry.get<MoneyInventory>(player);
                    playerMoney.money += 100;
                    registry.destroy(drop);
                }
                else if (dropName == "money200")
                {
                    auto& playerMoney = registry.get<MoneyInventory>(player);
                    playerMoney.money += 200;
                    registry.destroy(drop);
                }
                else if (dropName == "money500")
                {
                    auto& playerMoney = registry.get<MoneyInventory>(player);
                    playerMoney.money += 500;
                    registry.destroy(drop);
                }
                else if (dropName == "money1000")
                {
                    auto& playerMoney = registry.get<MoneyInventory>(player);
                    playerMoney.money += 1000;
                    registry.destroy(drop);
                }
                else
                {
                    //TODO: Log error
                }
            }
        }
    }
}

void destroyEntities(entt::registry& registry, std::unordered_set<entt::entity>& entitiesToDestroy)
{
    for (auto entity : entitiesToDestroy)
        registry.destroy(entity);
}

void CollisionSystem::updateCollisionBoxes(entt::registry& registry)
{
    auto view = registry.view<Collision, Position>();
    for (auto entity : view)
    {
        auto& collision = view.get<Collision>(entity);
        auto& position = view.get<Position>(entity);

        collision.collisionBox.left = position.position.x - collision.collisionBox.width / 2.f;
        collision.collisionBox.top = position.position.y - collision.collisionBox.height / 2.f;
    }
}

void CollisionSystem::checkCollisions(entt::registry& registry, sf::RenderWindow& window)
{
    std::unordered_set<entt::entity> entitiesToDestroy;

    checkBulletCollitions<PlayerBullet, Enemy>(registry, entitiesToDestroy, window);
    checkBulletCollitions<EnemyBullet, Player>(registry, entitiesToDestroy, window);

    checkDropCollision(registry);
    
    destroyEntities(registry, entitiesToDestroy);
}