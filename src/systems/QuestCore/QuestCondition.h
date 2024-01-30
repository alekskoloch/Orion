#pragma once

#include "../../pch.h"

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