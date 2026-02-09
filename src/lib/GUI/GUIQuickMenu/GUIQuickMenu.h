#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "ConfigManager.hpp"
#include "TextureManager.h"
#include "SoundManager.h"

#include "Mouse.h"
#include "GraphicsOperations.h"

#include "InputContext.hpp"

namespace Mouse
{
struct MouseState;
} // namespace Mouse

class Window;

class GUIQuickMenu
{
public:
    explicit GUIQuickMenu( entt::registry& registry );

    void setOnOpenCallback( std::function< void() > callback );
    void setOnCloseCallback( std::function< void() > callback );

    void onOpen();
    void onClose();

    void handleEvent( const InputContext& inputContext );
    void draw( Window& window );

    uint8_t getSelectedTile() const { return this->selectedTile; }

private:
    entt::registry& registry;

    std::vector<sf::Sprite> quickMenuTiles;
    std::vector<sf::Sprite> quickMenuIcons;

    uint8_t selectedTile = 0;

    void initializeQuickMenu();

    bool isOpen{ false };

    std::function< void() > m_onOpenCallback;
    std::function< void() > m_onCloseCallback;
};