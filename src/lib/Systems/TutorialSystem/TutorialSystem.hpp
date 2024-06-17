#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "EventManager.h"

#include "NotifySystem.h"
#include "QuestSystem.h"
#include "AreaGuardSystem.hpp"
#include "RemovalSystem.h"

class TutorialSystem
{
public:
    TutorialSystem(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry) { this->initialize(); }
    void clear();

    void update(sf::Time deltaTime, sf::RenderWindow& window, QuestSystem& questSystem, AreaGuardSystem& areaGuardSystem);
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    bool welcomeDisplayed = false;
    bool movingDisplayed = false;

    void initialize();

    void initializeGreetings();

    float timeSinceLastMessage = 0.0f;
    void updateTutorialTimers(sf::Time deltaTime);
};