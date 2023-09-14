#pragma once

#include "../pch.h"

#include "../managers/SceneManager.h"

#include "BackgroundManager.h"

// make this configurable
const constexpr float TARGET_SLOW_MOTION_FACTOR = 0.05f;
const constexpr float SLOW_MOTION_SPEED = 3.f;

class SystemManager
{
public:
    SystemManager(sf::RenderWindow& window, entt::registry& registry, sf::Event& event);

    void executeInitializationSystems();
    void executeEventSystems();
    void executeUpdateSystems(sf::Time deltaTime);
    void executeRenderSystems();

    void enableSlowMotion() { this->slowMotion = true; }
    void disableSlowMotion() { this->slowMotion = false; }

    bool debugMode = false;
private:
    sf::RenderWindow& window;
    entt::registry& registry;
    sf::Event& event;
    
    BackgroundManager backgroundManager;

    bool slowMotion = false;
    float slowMotionFactor = 1.0f;
};
