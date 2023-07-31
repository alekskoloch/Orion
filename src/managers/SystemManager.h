#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

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
    void updateSlowMotion(sf::Time deltaTime);

    sf::RenderWindow& window;
    entt::registry& registry;

    bool slowMotion = false;
    float slowMotionFactor = 1.0f;
    static constexpr float targetSlowMotionFactor = 0.05f;
    static constexpr float slowMotionSpeed = 3.f;
};
