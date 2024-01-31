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

    void update(entt::registry& registry, sf::Time deltaTime);
    std::vector<Quest>& getQuests();
private:
    std::vector<Quest> quests;
};