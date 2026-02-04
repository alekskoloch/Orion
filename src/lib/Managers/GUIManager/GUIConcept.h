#pragma once

class Window;

template < typename WidgetType >
concept GUIWidget = requires( WidgetType widget, Window& window )
{
    { widget.draw( window ) } -> std::same_as< void >;
};