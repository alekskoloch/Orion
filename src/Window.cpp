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

auto Window::isOpen() const -> bool
{
    return m_renderWindow->isOpen();
}

void Window::close() const
{
    m_renderWindow->close();
}

auto Window::getView() const -> const sf::View&
{
    return m_renderWindow->getView();
}

void Window::setView( const sf::View& view )
{
    m_renderWindow->setView( view );
}

void Window::setDefaultView() 
{
    m_renderWindow->setView( m_renderWindow->getDefaultView() );
}

auto Window::pollEvent() const -> std::optional< sf::Event >
{
    return m_renderWindow->pollEvent();
}

void Window::clear() const
{
    m_renderWindow->clear();
}

void Window::display() const
{
    m_renderWindow->display();
}

void Window::draw( const sf::Drawable& drawable )
{
    m_renderWindow->draw( drawable, sf::RenderStates::Default );
}

auto Window::getMouseState() const -> Mouse::MouseState
{
    const auto screenPosition = sf::Mouse::getPosition( *m_renderWindow );
    const auto worldPosition = m_renderWindow->mapPixelToCoords( screenPosition, m_renderWindow->getView() );

    return { .screenPosition = { .x = screenPosition.x, .y = screenPosition.y },
             .worldPosition = { .x = worldPosition.x, .y = worldPosition.y } };
}

void Window::setMouseCursor( const sf::Cursor& cursor )
{
    m_renderWindow->setMouseCursor( cursor );
}

auto Window::getWindow() -> sf::RenderWindow&
{
    return *m_renderWindow;
}