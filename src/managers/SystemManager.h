#pragma once

#include "../pch.h"

class SystemManager
{
public:
    SystemManager(sf::RenderWindow& window, entt::registry& registry);

    void executeInitializationSystems();
    void executeEventSystems();
    void executeUpdateSystems(sf::Time deltaTime);
    void executeRenderSystems();

    void enableSlowMotion() { this->slowMotion = true; }
    void disableSlowMotion() { this->slowMotion = false; }
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    bool slowMotion = false;
    //TODO: make this configurable
    float slowMotionFactor = 1.0f;
    static constexpr float targetSlowMotionFactor = 0.05f;
    static constexpr float slowMotionSpeed = 3.f;
};
