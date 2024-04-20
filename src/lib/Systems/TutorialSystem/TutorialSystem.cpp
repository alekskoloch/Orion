#include "pch.h"
#include "TutorialSystem.hpp"

#include "player.h"
#include "experience.h"

void TutorialSystem::clear()
{
    this->welcomeDisplayed = false;
}

void TutorialSystem::initialize()
{
    this->initializeGreetings();
}

void TutorialSystem::initializeGreetings()
{
    std::vector<std::string> welcomeMessage = {
        "Welcome to Orion! Prepare for an epic journey!",
        "These messages will guide you through the game, providing essential information along the way.",
        "You can disable tutorials in the settings at any time."
    };

    std::string buttonMessage = "Play";
    
    EventManager::getInstance().subscribe(EventManager::Event::Test, [this, welcomeMessage, buttonMessage]() {
        NotifySystem::notifyDialogBox(this->window, welcomeMessage, buttonMessage, [this]() {});
    });
}

void TutorialSystem::update(sf::Time deltaTime, sf::RenderWindow& window)
{
    auto playerView = this->registry.view<Player, Experience>();

    for (auto entity : playerView)
    {
        auto playerExp = playerView.get<Experience>(entity);

        if (!this->welcomeDisplayed)
        {
            this->welcomeDisplayed = true;

            EventManager::getInstance().trigger(EventManager::Event::Test);
        }
    }
}