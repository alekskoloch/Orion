#pragma once

#include "../../pch.h"

#include "QuestStage.h"

struct Quest
{
    std::string name;
    std::vector<QuestStage> stages;
    unsigned int currentStage = 0;
    bool completed = false;
    bool active = false;
};

class QuestBuilder
{
public:
    QuestBuilder() {}

    QuestBuilder& addName(std::string name)
    {
        quest.name = name;
        return *this;
    }
    QuestBuilder& addStage(QuestStage stage)
    {
        quest.stages.push_back(stage);
        return *this;
    }

    Quest build()
    {
        return quest;
    }
private:
    Quest quest;
};