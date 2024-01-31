#include "QuestSystem.h"

void QuestSystem::startQuest(Quest quest)
{
    NotifySystem::notify(NotifySystem::Type::BigInfo, "Quest started: " + quest.name);
    quests.push_back(quest);
}