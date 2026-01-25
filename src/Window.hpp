#pragma once

#include <SFML/Graphics.hpp>

class Window
{
public:
    Window();

    void initWindow();

    auto getWindow() -> sf::RenderWindow&{ return m_renderTarget; }

private:
    sf::RenderWindow m_renderTarget;
};