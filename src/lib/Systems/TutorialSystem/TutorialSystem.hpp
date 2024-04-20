#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "EventManager.h"

#include "NotifySystem.h"

class TutorialSystem
{
public:
    TutorialSystem(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry) { this->initialize(); }
    void clear();

    void update(sf::Time deltaTime, sf::RenderWindow& window);
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    bool welcomeDisplayed = false;

    void initialize();

    void initializeGreetings();
};