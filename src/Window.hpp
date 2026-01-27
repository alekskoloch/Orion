#pragma once

#include <memory>
#include <optional>

#include <SFML/System/Vector2.hpp>

namespace sf
{
class Cursor;
class Drawable;
class Event;
class RenderStates;
class RenderWindow;
class View;
} // namespace sf

class Window
{
public:
    Window();
    ~Window();

    Window( const Window& ) = delete;
    auto operator=( const Window& ) -> Window& = delete;

    Window( Window&& ) noexcept;
    auto operator=( Window&& ) noexcept -> Window&;

    void initWindow();

    [[nodiscard]] auto isOpen() const -> bool;
    void close() const;

    [[nodiscard]] auto getView() const -> const sf::View&;
    void setView( const sf::View& view );
    void setDefaultView();

    [[nodiscard]] auto pollEvent() const -> std::optional< sf::Event >;

    void clear() const;
    void display() const;
    void draw( const sf::Drawable& drawable );

    [[nodiscard]] auto getMousePosition() const -> sf::Vector2i;
    void setMouseCursor( const sf::Cursor& cursor );

    // TODO: remove after refactor, temporary solution
    auto getWindow() -> sf::RenderWindow&;

private:
    std::unique_ptr< sf::RenderWindow > m_renderWindow;
};