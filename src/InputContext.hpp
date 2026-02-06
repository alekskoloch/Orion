#pragma once

// SFML
#include <SFML/Window/Event.hpp>

// Engine
#include "InputData.hpp"

// TODO: needs cpp file, InputData should handle sf::Event also
class InputContext
{
public:
    InputContext( const sf::Event& event, const Mouse::MouseState& mouseState ) : m_event{ event }, m_mouseState{ mouseState } {}

    [[nodiscard]] auto getEvent() const -> const sf::Event&
    {
        return m_event.get();
    }

    [[nodiscard]] auto getMouseState() const -> const Mouse::MouseState&
    {
        return m_mouseState.get();
    }

private:
    std::reference_wrapper< const sf::Event > m_event;
    std::reference_wrapper< const Mouse::MouseState > m_mouseState;
};