#pragma once

#include "Button.hpp"
#include "Title.hpp"
#include "GUIBaseElement.h"

#include "GameState.hpp"

namespace sf
{
class Time;
} // namespace sf

struct InputContext;

class Window;

using MenuElement = GUIBaseElement< Button, Title >;

class MainMenu
{
public:
    MainMenu( GameState* gameState );

    void handleEvent( const InputContext& inputContext );
    void update( sf::Time deltaTime );
    void draw( Window& window );

private:
    void initButtons();

    GameState* m_gameState;

    std::vector< MenuElement > m_elements;
};