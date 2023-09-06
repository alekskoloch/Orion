#include "InputSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void InputSystem::processInput(entt::registry& registry)
{
    auto view = registry.view<Player, Input>();
    for (auto entity : view)
    {
        auto& input = view.get<Input>(entity);
        // TODO: Input keys should be configurable
        input.up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
        input.down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
        input.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
        input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

        input.shoot = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        input.getShield = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
    }
}