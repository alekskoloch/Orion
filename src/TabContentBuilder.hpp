#pragma once

#include <vector>
#include <string>
#include <functional>
#include <SFML/System/Vector2.hpp>

#include "TabWindow.hpp" 

class TabContentBuilder
{
public:
    explicit TabContentBuilder( const sf::Vector2f& startPosition, float spacing );

    TabContentBuilder& addButton( const std::string& label, const std::function<void()>& callback );
    TabContentBuilder& addTitle( const std::string& text );
    
    std::vector< TabChildElement > build();

private:
    sf::Vector2f m_currentPosition;
    float m_spacing;
    std::vector< TabChildElement > m_elements;
};