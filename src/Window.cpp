#include "Window.hpp"

#include <SFML/Graphics.hpp>

#include "ConfigManager.hpp"

Window::Window() : m_renderWindow{ std::make_unique< sf::RenderWindow >() }
{
    initWindow();
}

Window::~Window() = default;

Window::Window( Window&& ) noexcept = default;
auto Window::operator=( Window&& ) noexcept -> Window& = default;

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

    m_renderWindow->create( windowVideoMode, windowTitle, windowStyle, contextSettings );
    m_renderWindow->setFramerateLimit( windowFrameRateLimit );
}

auto Window::getWindow() -> sf::RenderWindow&
{
    return *m_renderWindow;
}