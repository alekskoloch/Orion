#pragma once

#include "../../pch.h"

#include "../../managers/EventManager.h"
#include "../PointSystem.h"

class IQuestCondition
{
public:
    virtual ~IQuestCondition() = default;
    virtual bool check(entt::registry& registry) = 0;

    virtual void subscribeEvent() = 0;
    virtual void unsubscribeEvent() = 0;

    virtual std::string getProgress() { return ""; }
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

private:
    int requiredKills;
    int currentKills = 0;
};