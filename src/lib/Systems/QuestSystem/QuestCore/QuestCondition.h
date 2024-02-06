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