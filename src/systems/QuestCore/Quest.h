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