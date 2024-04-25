#pragma once

#include "QuestStage.h"

enum class QuestType
{
    Common,
    Tutorial
};

struct Quest
{
    std::string name;
    std::vector<QuestStage> stages;
    unsigned int currentStage = 0;
    bool completed = false;
    bool active = false;
    QuestType type = QuestType::Common;
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

    QuestBuilder& setType(QuestType type)
    {
        quest.type = type;
        return *this;
    }

    Quest build()
    {
        return quest;
    }
private:
    Quest quest;
};