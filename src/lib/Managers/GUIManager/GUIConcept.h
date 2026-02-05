#pragma once

class Window;

constexpr size_t MAX_GUI_WIDGET_SIZE = 32768;

template < typename WidgetType >
concept IsGUIWidget = requires( WidgetType widget, Window& window ) {
    { widget.draw( window ) } -> std::same_as< void >;
};

template < typename GUIWidgetCandidate >
concept IsCacheFriendly = sizeof( GUIWidgetCandidate ) <= MAX_GUI_WIDGET_SIZE;