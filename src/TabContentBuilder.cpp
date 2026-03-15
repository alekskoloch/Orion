#include "TabContentBuilder.hpp"

TabContentBuilder::TabContentBuilder( const sf::Vector2f& startPosition, float spacing )
    : m_currentPosition( startPosition )
    , m_spacing( spacing )
{
}

TabContentBuilder& TabContentBuilder::addButton( const std::string& label, const std::function<void()>& callback )
{
    m_elements.emplace_back( Button( label, m_currentPosition, callback, Alignment::Center ) );
    m_currentPosition.y += m_spacing;
    return *this;
}

TabContentBuilder& TabContentBuilder::addTitle( const std::string& text )
{
    m_elements.emplace_back( Title( text, m_currentPosition, 40.0f ) ); 
    m_currentPosition.y += m_spacing;
    return *this;
}

std::vector< TabChildElement > TabContentBuilder::build()
{
    return std::move( m_elements );
}