#pragma once

#include "../pch.h"

#include "QuestCore/Quest.h"

#include "../systems/NotifySystem.h"

class QuestSystem
{
public:
    void startQuest(Quest quest);

private:
    std::vector<Quest> quests;
};