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
private:
    sf::RenderWindow& window;
    entt::registry& registry;
};
