#pragma once

#include "../pch.h"

#include "QuestCore/Quest.h"

#include "../systems/NotifySystem.h"
#include "../systems/ProceduralGenerationSystem.h"

class QuestSystem
{
public:
    void startQuest(Quest quest);
    void addRandomQuest(entt::registry& registry);

private:
    std::vector<Quest> quests;
};