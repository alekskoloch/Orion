#pragma once

#include <memory>

namespace sf
{
class RenderWindow;
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

    auto getWindow() -> sf::RenderWindow&;

private:
    std::unique_ptr< sf::RenderWindow > m_renderWindow;
};