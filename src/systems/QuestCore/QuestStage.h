#pragma once

#include "../../pch.h"

#include "QuestCondition.h"

struct QuestStage
{
    std::string description;
    std::function<void(entt::registry& registry)> action;
    std::shared_ptr<IQuestCondition> condition;

    bool actionExecuted = false;
    bool completed = false;
    bool isCompleted(entt::registry& registry)
    {
        return condition->check(registry);
    }
};