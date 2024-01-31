#include "QuestSystem.h"

void QuestSystem::startQuest(Quest quest)
{
    NotifySystem::notify(NotifySystem::Type::BigInfo, "Quest started: " + quest.name);
    quests.push_back(quest);
}

void QuestSystem::addRandomQuest(entt::registry& registry)
{
    QuestBuilder builder;

    int stagesCount = ProceduralGenerationSystem::GetRandomNumber(3, 7);
    
    auto stageIndices = std::views::iota(0, stagesCount);
    std::ranges::for_each(stageIndices, [&builder, &registry](int i) {
        if (ProceduralGenerationSystem::GetRandomNumber(0, 1) == 0)
        {
            auto reachCondition = std::make_shared<ReachPointOfInterestCondition>("randomQuestPointOfInterest" + std::to_string(i));
            QuestStage stage = QuestStageBuilder()
                .addDescription("Find the point of interest")
                .addAction([i](entt::registry& reg) {
                    PointSystem::addPointOfInterest(reg, sf::Vector2f(ProceduralGenerationSystem::GetRandomNumber(-5000, 5000), ProceduralGenerationSystem::GetRandomNumber(-5000, 5000)), "randomQuestPointOfInterest" + std::to_string(i));
                })
                .addCondition(reachCondition)
                .build();
            builder.addStage(stage);
        }
        else
        {
            unsigned int enemiesCount = ProceduralGenerationSystem::GetRandomNumber(1, 10);
            auto killCondition = std::make_shared<KillEnemiesCondition>(enemiesCount);
            QuestStage stage = QuestStageBuilder()
                .addDescription("Kill " + std::to_string(enemiesCount) + " enemies")
                .addAction([](entt::registry& reg) {
                })
                .addCondition(killCondition)
                .build();
            builder.addStage(stage);
        }
    });

    builder.addName("Random quest");
    Quest quest = builder.build();
    this->startQuest(quest);
}

void QuestSystem::update(entt::registry& registry, sf::Time deltaTime)
{
    // Execute current stage action of each quest
    for (auto& quest : quests)
    {
        if (!quest.completed)
        {
            auto& currentStage = quest.stages[quest.currentStage];
            if (!currentStage.completed && !currentStage.actionExecuted)
            {
                currentStage.action(registry);
                currentStage.actionExecuted = true;
                currentStage.condition->subscribeEvent();
            }
        }
    }

    // Checking for completed quests
    for (auto& quest : quests)
    {
        if (!quest.completed)
        {
            auto& currentStage = quest.stages[quest.currentStage];
            if (!currentStage.completed && currentStage.condition->check(registry))
            {
                currentStage.completed = true;
                currentStage.condition->unsubscribeEvent();
                quest.currentStage++;
                if (quest.currentStage == quest.stages.size())
                {
                    quest.completed = true;
                    quest.active = false;
                    NotifySystem::notify(NotifySystem::Type::BigInfo, "Quest completed: " + quest.name);
                }
                else
                {
                    NotifySystem::notify(NotifySystem::Type::Info, quest.name + ": New journal entry.");
                }
            }
        }
    }
}

std::vector<Quest>& QuestSystem::getQuests()
{
    return this->quests;
}