#include "TabContentBuilder.hpp"
#include "Slider.hpp"
#include "SettingSection.hpp"

TabContentBuilder::TabContentBuilder( const sf::Vector2f& startPosition, float spacing )
    : m_currentPosition( startPosition )
    , m_spacing( spacing )
{
}

TabContentBuilder& TabContentBuilder::addSettingSection( const std::string& label )
{
    m_elements.emplace_back( SettingSection( label, m_currentPosition ) );
    m_currentSectionIndex = m_elements.size() - 1;
    m_currentPosition.y += m_spacing;
    return *this;
}

TabContentBuilder& TabContentBuilder::addButton( const std::string& label, const std::function<void()>& callback )
{
    if ( m_currentSectionIndex.has_value() )
    {
        std::visit( [ &label, &callback ]( auto& element )
        {
            if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( element ) >, SettingSection > )
            {
                element.setButton( label, callback );
            }
        }, m_elements[ m_currentSectionIndex.value() ] );
    }
    else
    {
        m_elements.emplace_back( Button( label, m_currentPosition, callback, Alignment::Center ) );
        m_currentPosition.y += m_spacing;
    }
    return *this;
}

TabContentBuilder& TabContentBuilder::addTitle( const std::string& text )
{
    m_currentSectionIndex.reset();
    m_elements.emplace_back( Title( text, m_currentPosition, 40.0f ) );
    m_currentPosition.y += m_spacing;
    return *this;
}

TabContentBuilder& TabContentBuilder::addSlider( const std::function<void( float )>& callback,
                                                  float minValue, float maxValue, float initialValue, ShowValue showValue )
{
    if ( m_currentSectionIndex.has_value() )
    {
        std::visit( [ &callback, minValue, maxValue, initialValue, showValue ]( auto& element )
        {
            if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( element ) >, SettingSection > )
            {
                element.setSlider( callback, minValue, maxValue, initialValue, showValue );
            }
        }, m_elements[ m_currentSectionIndex.value() ] );
    }
    else
    {
        m_elements.emplace_back( Slider( m_currentPosition, callback, minValue, maxValue, initialValue, showValue ) );
        m_currentPosition.y += m_spacing;
    }
    return *this;
}

std::vector< TabChildElement > TabContentBuilder::build()
{
    m_currentSectionIndex.reset();
    return std::move( m_elements );
}