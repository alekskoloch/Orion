#include "pch.h"
#include "InputSystem.h"

void InputSystem::processInput(entt::registry& registry, sf::Event& event)
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

        input.specialShot = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);

        if (!input.getShield)
            input.isGettingShield = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
        else
            input.isGettingShield = false;

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Right)
        {
            input.getShield = true;
            input.canGetShield = true;
        }
        else
            input.getShield = false;

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::LAlt && input.movementBoostActive == false)
            input.readyForMovementBoost = true;
        else
            input.readyForMovementBoost = false;

        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Key::LAlt)
            input.movementBoostActive = false;
    }
}