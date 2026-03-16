#pragma once

#include <vector>
#include <string>
#include <functional>
#include <optional>
#include <SFML/System/Vector2.hpp>

#include "TabWindow.hpp"

class TabContentBuilder
{
public:
    explicit TabContentBuilder( const sf::Vector2f& startPosition, float spacing );

    TabContentBuilder& addSettingSection( const std::string& label );

    TabContentBuilder& addButton( const std::string& label, const std::function<void()>& callback );
    TabContentBuilder& addTitle( const std::string& text );
    TabContentBuilder& addSlider( const std::function<void( float )>& callback,
                                   float minValue = 0.0f, float maxValue = 100.0f, float initialValue = 50.0f, ShowValue showValue = ShowValue::Yes );

    std::vector< TabChildElement > build();

private:
    sf::Vector2f m_currentPosition;
    float m_spacing;
    std::vector< TabChildElement > m_elements;
    std::optional< std::size_t > m_currentSectionIndex;
};