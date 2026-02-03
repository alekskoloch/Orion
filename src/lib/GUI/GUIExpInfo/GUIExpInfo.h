#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

class Window;

class GUIExpInfo : public sf::Drawable
{
public:
    explicit GUIExpInfo( entt::registry& registry );

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void draw( Window& window );
private:
    entt::registry& registry;

    sf::RectangleShape backgroundBar;
    sf::RectangleShape expBar;
    bool skillPointsAvailableVisible = false;
    sf::CircleShape skillPointsAvailable;
};