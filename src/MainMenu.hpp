#pragma once

namespace sf
{
class Time;
} // namespace sf

struct InputContext;

class Window;

class MainMenu
{
public:
    void handleInput( const InputContext& inputContext );
    void update( sf::Time deltaTime );
    void draw( Window& window );
};