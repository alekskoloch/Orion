#pragma once

// STD
#include <variant>

// SFML
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

// Engine
#include "GUIConcept.h"

namespace Mouse
{
struct MouseState;
} // namespace Mouse

template < typename... WidgetTypes >
    requires( ( IsGUIWidget< WidgetTypes > && IsCacheFriendly< WidgetTypes > ) && ... )
class GUIBaseElement
{
public:
    template < typename WidgetType >
        requires( ( std::same_as< std::remove_cvref_t< WidgetType >, WidgetTypes > || ... ) )
    constexpr explicit GUIBaseElement( WidgetType&& widget ) : m_storage( std::forward< WidgetType >( widget ) ){};

    void update( sf::Time deltaTime, const sf::Event event, const Mouse::MouseState& mouseState )
    {
        std::visit( [ deltaTime, &event, &mouseState ]( auto& element )
                    { dispatchUpdate( element, deltaTime, event, mouseState ); }, m_storage );
    }

    void draw( Window& window )
    {
        std::visit( [ &window ]( auto& element ) { element.draw( window ); }, m_storage );
    }

private:
    // clang-format off
    template < typename WidgetType >
    static void dispatchUpdate( WidgetType& widget, sf::Time deltaTime, const sf::Event& event, const Mouse::MouseState& mouseState )
    {
        if constexpr ( requires { widget.update( deltaTime, event, mouseState ); } ) { widget.update( deltaTime, event, mouseState ); }
        else if constexpr ( requires { widget.update( deltaTime, mouseState ); } ) { widget.update( deltaTime, mouseState ); }
        else if constexpr ( requires { widget.update( mouseState ); } ) { widget.update( mouseState ); }
        else if constexpr ( requires { widget.update( deltaTime ); } ) { widget.update( deltaTime ); }
        else if constexpr ( requires { widget.update( deltaTime, event ); } ) { widget.update( deltaTime, event ); }
        else if constexpr ( requires { widget.update( event ); } ) { widget.update( event ); }
        else if constexpr ( requires { widget.update(); } ) { widget.update(); }
    }
    // clang-format on

    std::variant< WidgetTypes... > m_storage;
};