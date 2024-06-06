#pragma once

#include "EventManager.h"

#include "PointSystem.h"

#include "pointOfInterest.h"
#include "position.h"
#include "player.h"

#include "MathOperations.h"

class IQuestCondition
{
public:
    virtual ~IQuestCondition() = default;
    virtual bool check(entt::registry& registry) = 0;

    virtual void subscribeEvent() = 0;
    virtual void unsubscribeEvent() = 0;

    virtual std::string getProgress() { return ""; }
    virtual float getTargetDistance(entt::registry& registry) { return 0.0f; }
    virtual sf::Vector2f getTargetPosition(entt::registry& registry) { return sf::Vector2f(); }
protected:
    unsigned int subscriberId;
};

class DefaultCondition : public IQuestCondition
{
public:
    bool check(entt::registry& registry) override
    {
        return true;
    }

    void subscribeEvent() override
    {
        
    }

    void unsubscribeEvent() override
    {
        
    }

    std::string getProgress()
    {
        return "No condition";
    }

    float getTargetDistance(entt::registry& registry) override
    {
        return 0.0f;
    }

    sf::Vector2f getTargetPosition(entt::registry& registry) override
    {
        return sf::Vector2f();
    }
};

class ReachPointOfInterestCondition : public IQuestCondition
{
public:
    ReachPointOfInterestCondition(std::string pointOfInterestName) : pointOfInterestName(pointOfInterestName) {}

    bool check(entt::registry& registry) override
    {
        return !PointSystem::isPointOfInterestActive(registry, pointOfInterestName);
    }

    void subscribeEvent() override
    {
        
    }

    void unsubscribeEvent() override
    {
        
    }

    std::string getProgress()
    {
        return "You must reach the point!";
    }

    float getTargetDistance(entt::registry& registry) override
    {
        auto view = registry.view<PointOfInterest, Position>();
        for (auto entity : view)
        {
            auto& pointOfInterest = view.get<PointOfInterest>(entity);
            if (pointOfInterest.id == pointOfInterestName)
            {
                auto& position = view.get<Position>(entity);
                auto playerEntity = registry.view<Player>()[0];
                auto playerPosition = registry.get<Position>(playerEntity);

                return CalculateDistance(position.position, playerPosition.position);
            }
        }
        return 0.0f;
    }

    sf::Vector2f getTargetPosition(entt::registry& registry) override
    {
        auto view = registry.view<PointOfInterest, Position>();
        for (auto entity : view)
        {
            auto& pointOfInterest = view.get<PointOfInterest>(entity);
            if (pointOfInterest.id == pointOfInterestName)
            {
                auto& position = view.get<Position>(entity);
                return position.position;
            }
        }
        return sf::Vector2f();
    }

private:
    std::string pointOfInterestName;
};

class KillEnemiesCondition : public IQuestCondition
{
public:
    KillEnemiesCondition(int requiredKills) : requiredKills(requiredKills) {}

    void subscribeEvent() override
    {
        this->subscriberId = EventManager::getInstance().subscribe(EventManager::Event::EnemyKilled, [this]() {
            currentKills++;
        });
    }

    void unsubscribeEvent() override
    {
        EventManager::getInstance().unsubscribe(this->subscriberId);
    }

    bool check(entt::registry& registry) override
    {
        return currentKills >= requiredKills;
    }

    std::string getProgress()
    {
        return std::string("Enemy to kill: ") + std::to_string(currentKills) + "/" + std::to_string(requiredKills);
    }

    float getTargetDistance(entt::registry& registry) override
    {
        return 0.0f;
    }

    sf::Vector2f getTargetPosition(entt::registry& registry) override
    {
        return sf::Vector2f();
    }

private:
    int requiredKills;
    int currentKills = 0;
};

class KeyPressedCondition : public IQuestCondition
{
public:
    KeyPressedCondition(sf::Keyboard::Key key) : key(key) {}

    void subscribeEvent() override
    {

    }

    void unsubscribeEvent() override
    {
    
    }

    bool check(entt::registry& registry) override
    {
        return sf::Keyboard::isKeyPressed(key);
    }

    std::string getProgress()
    {
        return "Press the key!";
    }

    float getTargetDistance(entt::registry& registry) override
    {
        return 0.0f;
    }

    sf::Vector2f getTargetPosition(entt::registry& registry) override
    {
        return sf::Vector2f();
    }

private:
    sf::Keyboard::Key key;
};

class PlayerShieldActiveCondition : public IQuestCondition
{
public:
    PlayerShieldActiveCondition() {}

    void subscribeEvent() override
    {
        this->subscriberId = EventManager::getInstance().subscribe(EventManager::Event::ShieldActivated, [this]() {
            this->shieldActive = true;
        });
    }

    void unsubscribeEvent() override
    {
        EventManager::getInstance().unsubscribe(this->subscriberId);
    }

    bool check(entt::registry& registry) override
    {
        return shieldActive;
    }

    std::string getProgress()
    {
        return "Shield active!";
    }

    float getTargetDistance(entt::registry& registry) override
    {
        return 0.0f;
    }

    sf::Vector2f getTargetPosition(entt::registry& registry) override
    {
        return sf::Vector2f();
    }

private:
    bool shieldActive = false;
};

class PlayerShieldDestroyedCondition : public IQuestCondition
{
public:
    PlayerShieldDestroyedCondition() {}

    void subscribeEvent() override
    {
        this->subscriberId = EventManager::getInstance().subscribe(EventManager::Event::ShieldDestroyed, [this]() {
            this->shieldDestroyed = true;
        });
    }

    void unsubscribeEvent() override
    {
        EventManager::getInstance().unsubscribe(this->subscriberId);
    }

    bool check(entt::registry& registry) override
    {
        return shieldDestroyed;
    }

    std::string getProgress()
    {
        return "Shield destroyed!";
    }

    float getTargetDistance(entt::registry& registry) override
    {
        return 0.0f;
    }

    sf::Vector2f getTargetPosition(entt::registry& registry) override
    {
        return sf::Vector2f();
    }

private:
    bool shieldDestroyed = false;
};  