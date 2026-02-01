#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "ConfigManager.hpp"
#include "TextureManager.h"
#include "SoundManager.h"

#include "Mouse.h"
#include "GraphicsOperations.h"

namespace Mouse
{
struct MouseState;
} // namespace Mouse

class Window;

class GUIQuickMenu
{
public:
    GUIQuickMenu( entt::registry& registry );

    void update( const Mouse::MouseState& mouseState );
    void draw( Window& window );

    uint8_t getSelectedTile() const { return this->selectedTile; }

private:
    entt::registry& registry;

    std::vector<sf::Sprite> quickMenuTiles;
    std::vector<sf::Sprite> quickMenuIcons;

    uint8_t selectedTile = 0;

    void initializeQuickMenu();
};