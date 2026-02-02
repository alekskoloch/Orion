#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Window.hpp"
#include "GUIElement.hpp"

namespace Mouse
{
struct MouseState;
} // namespace Mouse

class GUISettings
{
public:
    GUISettings();

    void update( const Mouse::MouseState& mouseState, sf::Time& deltaTime );
    void draw( Window& window );
private:
    sf::View view;

    std::vector<std::unique_ptr<GUIElement>> elements;

    void initialize();
    unsigned int SCREEN_WIDTH;
    unsigned int SCREEN_HEIGHT;

    unsigned int BUTTON_WIDTH;
    unsigned int BUTTON_HEIGHT;

    unsigned int TITLE_FONT_SIZE;
    unsigned int BUTTONS_FONT_SIZE;

    float MARGIN;
    float OUTLINE_THICKNESS;

    std::string loadedResolution;
    std::string loadedFrameRate;
    std::string loadedWindowMode;
};