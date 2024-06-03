#include "pch.h"
#include "TutorialSystem.hpp"

#include "ConfigManager.hpp"

#include "EnemyInitializationSystem.h"

#include "player.h"
#include "experience.h"

void TutorialSystem::clear()
{
    this->initializeGreetings();
}

void TutorialSystem::initialize()
{
    if (ConfigManager::getInstance().isTutorialEnabled())
        this->initializeGreetings();
}

void TutorialSystem::initializeGreetings()
{
    this->welcomeDisplayed = false;
    this->movingDisplayed = false;

    std::vector<std::string> welcomeMessage = {
        "Welcome to Orion! Prepare for an epic journey!",
        "These messages will guide you through the game, providing essential information along the way.",
        "You can disable tutorials in the settings at any time."
    };

    std::string buttonMessage = "Play";
    
    EventManager::getInstance().subscribe(EventManager::Event::Tutorial, [this, welcomeMessage, buttonMessage]() {
        NotifySystem::notifyDialogBox(this->window, welcomeMessage, buttonMessage, [this]() {});
    });
}

void TutorialSystem::update(sf::Time deltaTime, sf::RenderWindow& window, QuestSystem& questSystem)
{
    this->timeSinceLastMessage += deltaTime.asSeconds();

    if (ConfigManager::getInstance().isTutorialEnabled())
    {
        auto playerView = this->registry.view<Player, Experience>();

        for (auto entity : playerView)
        {
            auto playerExp = playerView.get<Experience>(entity);

            if (!this->welcomeDisplayed)
            {
                this->welcomeDisplayed = true;

                EventManager::getInstance().trigger(EventManager::Event::Tutorial);
                this->timeSinceLastMessage = 0.0f;
            }
        }

        if (this->timeSinceLastMessage > 0.5f && this->welcomeDisplayed && !this->movingDisplayed)
        {
            this->movingDisplayed = true;
            this->timeSinceLastMessage = 0.0f;

            std::vector<std::string> movingMessage = {
                "Use the WASD keys to move.",
                "Try to reach the point of interest!"
            };

            std::string buttonMessage = "Got it";

            NotifySystem::notifyDialogBox(this->window, movingMessage, buttonMessage, [this]() {});

            QuestStage stage = QuestStageBuilder()
                .addDescription("Find the point of interest")
                .addAction([](entt::registry& reg) {
                    int x = 1000;
                    int y = 1000;
                    PointSystem::addPointOfInterest(reg, sf::Vector2f(x, y), "TutorialPoint1", PointOfInterestType::Tutorial);
                })
                .addCondition(std::make_shared<ReachPointOfInterestCondition>("TutorialPoint1"))
                .build();

            std::vector<std::string> movingMessage2 = {
                "Excellent! You've reached the point of interest!",
                "Now, find the next one!"
            };
            
            QuestStage stage2 = QuestStageBuilder()
                .addDescription("Find the point of interest")
                .addAction([&window, movingMessage2](entt::registry& reg) {
                    int x = -1000;
                    int y = -1000;
                    PointSystem::addPointOfInterest(reg, sf::Vector2f(x, y), "TutorialPoint2", PointOfInterestType::Tutorial);
                    NotifySystem::notifyDialogBox(window,
                        movingMessage2,
                        "OK", []() {}
                    );
                })
                .addCondition(std::make_shared<ReachPointOfInterestCondition>("TutorialPoint2"))
                .build();

            QuestStage stage3 = QuestStageBuilder()
                .addDescription("Kill an enemy")
                .addAction([&window](entt::registry& reg) {
                    NotifySystem::notifyDialogBox(window,
                        std::vector<std::string>
                            {
                                "Great job! You've reached the second point of interest!",
                                "Now, destroy all targets to proceed!",
                                "You can use the mouse to aim and the left mouse button to shoot."
                            },
                        "OK", []() {}
                    );

                    // TODO: This should be in some tutorial configuration file
                    const float RADIUS = 1500;
                    const unsigned int ENEMIES_COUNT = 8;
                    
                    for (int i = 0; i < ENEMIES_COUNT; i++)
                    {
                        float angle = i * 45;
                        float x = -1000 + RADIUS * cos(angle * 3.14159265 / 180);
                        float y = -1000 + RADIUS * sin(angle * 3.14159265 / 180);

                        EnemyInitializationSystem::spawnEnemy(reg, sf::Vector2f(x, y), "Target");
                    }
                })
                .addCondition(std::make_shared<KillEnemiesCondition>(8))
                .build();

            QuestStage endStage = QuestStageBuilder()
                .addDescription("Congratulations!")
                .addAction([&window](entt::registry& reg) {
                    NotifySystem::notifyDialogBox(window,
                        std::vector<std::string>
                            {
                                "Congratulations! You've completed the tutorial!",
                                "You're now ready to explore the world of Orion!"
                            },
                        "OK", []() {}
                    );

                    EventManager::getInstance().trigger(EventManager::Event::EnableEnemySpawning);
                })
                .addCondition(std::make_shared<DefaultCondition>())
                .build();

            QuestBuilder questBuilder;
            questBuilder.addName("Tutorial");
            questBuilder.addStage(stage);
            questBuilder.addStage(stage2);
            questBuilder.addStage(stage3);
            questBuilder.addStage(endStage);
            questBuilder.setType(QuestType::Tutorial);

            Quest quest = questBuilder.build();
            questSystem.startQuest(quest);
        }
    }
}