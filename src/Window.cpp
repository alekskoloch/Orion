#include "Window.hpp"

#include "ConfigManager.hpp"

Window::Window()
{
    initWindow();
}

void Window::initWindow()
{
    const auto& configManagerInstance{ ConfigManager::getInstance() };

    const auto windowSize{ sf::Vector2u{ configManagerInstance.getScreenWidth(),
                                         configManagerInstance.getScreenHeight() } };

    const auto windowVideoMode{ sf::VideoMode{ windowSize } };
    const auto windowTitle{ configManagerInstance.getWindowTitle() };
    const auto windowStyle{ configManagerInstance.getWindowStyle() };
    const auto windowFrameRateLimit{ configManagerInstance.getFrameRateLimit() };
    const auto windowAntialiasingLevel{ configManagerInstance.getWindowAntialiasing() };

    sf::ContextSettings contextSettings;
    contextSettings.antiAliasingLevel = windowAntialiasingLevel;

    m_renderTarget.create( windowVideoMode, windowTitle, windowStyle, contextSettings );
    m_renderTarget.setFramerateLimit( windowFrameRateLimit );
}