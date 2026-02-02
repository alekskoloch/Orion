#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

class Window;

namespace Mouse
{
struct MouseState;
} // namespace Mouse

class GUISkillTree
{
public:
    explicit GUISkillTree( entt::registry& registry );

    void update( sf::Time& deltaTime, const Mouse::MouseState& mouseState );
    void draw( Window& window );

    auto getView() const -> const sf::View&;
private:
    entt::registry& registry;
    sf::View view;
    float moveSpeed = 1000.0f;

    sf::Font font;
};