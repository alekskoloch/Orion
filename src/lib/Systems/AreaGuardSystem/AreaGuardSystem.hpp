#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

class AreaGuardSystem
{
public:
    AreaGuardSystem(entt::registry& registry);

    void setGuardArea(sf::FloatRect guardArea);
    void disableGuardArea();

    void update(sf::Time deltaTime);

    void draw(sf::RenderWindow& window);
private:
    entt::registry& registry;
    sf::FloatRect guardArea;

    bool active = false;

    sf::RectangleShape areaRectangle;
    void initializeAreaRectangle();
};