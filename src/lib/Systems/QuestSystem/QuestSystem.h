#pragma once

#include <entt/entt.hpp>
#include <SFML/System.hpp>
#include "QuestCore/Quest.h"
#include "NotifySystem.h"
#include "ProceduralGenerationSystem.h"
#include "StringOperations.h"

class QuestSystem
{
public:
    void startQuest(Quest quest);
    void addRandomQuest(entt::registry& registry, std::string questName);

    void update(entt::registry& registry, sf::Time deltaTime);
    std::vector<Quest>& getQuests();
private:
    std::vector<Quest> quests;
};