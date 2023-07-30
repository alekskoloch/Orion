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
    sf::RenderWindow& window;
    entt::registry& registry;

    bool slowMotion = false;
};
