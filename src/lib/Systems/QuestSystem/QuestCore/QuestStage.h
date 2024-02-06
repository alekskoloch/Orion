#pragma once

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

class QuestStageBuilder
{
public:
    QuestStageBuilder() {}

    QuestStageBuilder& addDescription(std::string description)
    {
        stage.description = description;
        return *this;
    }
    QuestStageBuilder& addAction(std::function<void(entt::registry& registry)> action)
    {
        stage.action = action;
        return *this;
    }
    QuestStageBuilder& addCondition(const std::shared_ptr<IQuestCondition>& condition)
    {
        stage.condition = condition;
        return *this;
    }
    
    QuestStage build()
    {
        return stage;
    }
private:
    QuestStage stage;
};