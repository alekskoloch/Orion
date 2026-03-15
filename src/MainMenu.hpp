#pragma once

#include "Button.hpp"
#include "Title.hpp"
#include "GUIBaseElement.h"

#include "GameState.hpp"

#include "MenuElement.hpp"

namespace sf
{
class Time;
} // namespace sf

struct InputContext;

class Window;

using MenuElement = GUIBaseElement< Button, Title, TabWindow >;

enum class MenuState : std::uint8_t
{
    MainMenu,
    LoadGame,
    Settings
};

class MainMenu
{
public:
    explicit MainMenu( GameState* gameState );

    void handleEvent( const InputContext& inputContext );
    void update( sf::Time deltaTime );
    void draw( Window& window );

private:
    void initMainMenu();
    void initLoadGame();
    void initSettings();

    GameState* m_gameState;

    MenuState m_menuState{ MenuState::MainMenu };
    std::vector< MenuElement > m_mainMenuElements;
    std::vector< MenuElement > m_settingsElements;
    std::vector< MenuElement > m_loadGameElements;
};