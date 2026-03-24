#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <algorithm>
#include <cmath>

#include "ConfigManager.hpp"
#include "InputContext.hpp"
#include "Window.hpp"

class Scrollbar
{
public:
    static constexpr float DEFAULT_WIDTH = 12.0f;
    static constexpr float MIN_THUMB_HEIGHT = 40.0f;
    static constexpr float SCROLL_SENSITIVITY = 40.0f;  // Pixels per scroll wheel unit
    static constexpr float SCROLL_SMOOTH_SPEED = 8.0f;  // Smooth scroll interpolation speed

    Scrollbar( sf::Vector2f position, float height )
        : m_font( std::make_shared< sf::Font >() )
        , m_trackHeight( height )
        , m_position( position )
        , m_dragOffset( 0.0f )
    {
        float scale = ConfigManager::getInstance().getScale();
        m_scale = scale;

        std::string fontPath = std::string( ASSETS_PATH ) + "fonts/ScienceGothic-Regular.ttf";
        if ( !m_font->openFromFile( fontPath ) )
        {
            std::cerr << "ERROR: Scrollbar font load failed: " << fontPath << std::endl;
        }

        m_trackSize = { DEFAULT_WIDTH * scale, m_trackHeight };
        m_track.setSize( m_trackSize );
        m_track.setOrigin( { m_trackSize.x / 2.0f, 0.0f } );
        m_track.setPosition( { position } );
        m_track.setFillColor( sf::Color( 10, 10, 15 ) );

        std::string shaderPath = std::string( ASSETS_PATH ) + "shaders/scrollbar.frag";
        if ( !m_shader.loadFromFile( shaderPath, sf::Shader::Type::Fragment ) )
        {
            std::cerr << "CRITICAL: Scrollbar shader load failed: " << shaderPath << std::endl;
        }

        updateThumbGeometry();
    }

    void setContentHeight( float contentHeight, float viewportHeight )
    {
        m_contentHeight = contentHeight;
        m_viewportHeight = viewportHeight;

        // Reset scroll offset when content changes or if content now fits
        m_scrollOffset = 0.0f;
        m_scrollOffsetTarget = 0.0f;
        m_normalizedScroll = 0.0f;
        
        // Update thumb geometry - will position thumb at very top
        updateThumbGeometry();
    }

    float getScrollOffset() const
    {
        return m_scrollOffset;
    }

    void resetScrollToTop()
    {
        m_scrollOffset = 0.0f;
        m_scrollOffsetTarget = 0.0f;
        m_normalizedScroll = 0.0f;
    }

    void handleEvent( const InputContext& inputContext, bool forceScroll = false )
    {
        const auto& event = inputContext.getEvent();
        const auto& mouseState = inputContext.getMouseState();
        sf::Vector2f mousePos{ mouseState.worldPosition.x, mouseState.worldPosition.y };

        if ( m_contentHeight <= m_viewportHeight )
        {
            return;
        }

        // If forceScroll is true, always process scroll wheel (mouse anywhere in tab)
        // Otherwise, only process when mouse is over the track
        bool isHovered = forceScroll || isPointInsideTrack( mousePos );
        m_isHovered = isHovered;

        bool isThumbHovered = isPointInsideThumb( mousePos );
        m_isThumbHovered = isThumbHovered;

        if ( m_isDragging )
        {
            if ( const auto* released = event.getIf< sf::Event::MouseButtonReleased >() )
            {
                if ( released->button == sf::Mouse::Button::Left )
                {
                    m_isDragging = false;
                }
            }
            else
            {
                // Apply drag offset to maintain relative position within thumb
                float adjustedMouseY = mousePos.y - m_dragOffset;
                
                float trackTop = m_position.y + ( m_thumbHeight / 2.0f );
                float trackBottom = m_position.y + m_trackHeight - ( m_thumbHeight / 2.0f );
                
                float clampedY = std::clamp( adjustedMouseY, trackTop, trackBottom );
                float thumbScrollableDistance = m_trackHeight - m_thumbHeight;

                if ( thumbScrollableDistance > 0.0f )
                {
                    // Calculate normalized scroll from clamped position
                    float normalizedFromPos = ( clampedY - trackTop ) / thumbScrollableDistance;
                    m_normalizedScroll = std::clamp( normalizedFromPos, 0.0f, 1.0f );
                }
                else
                {
                    m_normalizedScroll = 0.0f;
                }

                float contentScrollableDistance = m_contentHeight - m_viewportHeight;
                m_scrollOffsetTarget = m_normalizedScroll * contentScrollableDistance;
                m_scrollOffset = m_scrollOffsetTarget;  // When dragging, update immediately (no smoothing)
            }
        }
        else if ( isThumbHovered )
        {
            // Only allow dragging when hovering over the thumb itself
            if ( const auto* pressed = event.getIf< sf::Event::MouseButtonPressed >() )
            {
                if ( pressed->button == sf::Mouse::Button::Left )
                {
                    m_isDragging = true;
                    // Store the offset between mouse and thumb center to prevent jumping
                    m_dragOffset = mousePos.y - m_thumbPosition.y;
                }
            }
        }

        // Scroll wheel always works when forceScroll is true (mouse anywhere in tab)
        if ( forceScroll )
        {
            if ( const auto* scrolled = event.getIf< sf::Event::MouseWheelScrolled >() )
            {
                float scrollStep = SCROLL_SENSITIVITY * m_scale;
                float contentScrollableDistance = m_contentHeight - m_viewportHeight;

                if ( contentScrollableDistance > 0.0f )
                {
                    float delta = -scrolled->delta * scrollStep;
                    m_scrollOffsetTarget = std::clamp( m_scrollOffsetTarget + delta, 0.0f, contentScrollableDistance );
                    // Don't update m_scrollOffset here - it will be smoothed in update()
                }
            }
        }

        m_mouseLocalPos = mousePos - m_position;
    }

    void update( sf::Time deltaTime )
    {
        // Smooth scroll interpolation - same pattern as CameraSystem
        if ( m_scrollOffset != m_scrollOffsetTarget )
        {
            float scrollDelta = m_scrollOffsetTarget - m_scrollOffset;
            float scrollChange = scrollDelta * deltaTime.asSeconds() * SCROLL_SMOOTH_SPEED;
            m_scrollOffset += scrollChange;

            // Check if we're close enough to snap to target
            if ( std::abs( scrollDelta ) < 0.5f )
            {
                m_scrollOffset = m_scrollOffsetTarget;
            }
        }

        // Update thumb position based on current (smoothed) scroll offset
        float contentScrollableDistance = m_contentHeight - m_viewportHeight;

        // Only calculate thumb position if there's actual scrolling to do
        if ( contentScrollableDistance > 1.0f )
        {
            m_normalizedScroll = m_scrollOffset / contentScrollableDistance;
            float thumbScrollableDistance = m_trackHeight - m_thumbHeight;
            // Thumb position is its CENTER, so add half thumb height
            float thumbY = m_position.y + ( m_normalizedScroll * thumbScrollableDistance ) + ( m_thumbHeight / 2.0f );
            m_thumbPosition = { m_position.x, std::clamp( thumbY, m_position.y + ( m_thumbHeight / 2.0f ), m_position.y + m_trackHeight - ( m_thumbHeight / 2.0f ) ) };
        }
        else
        {
            // No scrolling needed - thumb fills entire track, center at track center
            m_normalizedScroll = 0.0f;
            m_thumbPosition = m_position + sf::Vector2f( 0.0f, m_trackHeight / 2.0f );
        }

        m_shader.setUniform( "u_time", m_clock.getElapsedTime().asSeconds() );
        m_shader.setUniform( "u_hover", m_isHovered ? 1.0f : 0.0f );
        m_shader.setUniform( "u_thumbHover", m_isThumbHovered ? 1.0f : 0.0f );
        m_shader.setUniform( "u_dragging", m_isDragging ? 1.0f : 0.0f );
    }

    void draw( Window& window )
    {
        if ( m_contentHeight <= m_viewportHeight )
        {
            return;
        }

        float scale = ConfigManager::getInstance().getScale();

        m_shader.setUniform( "u_pos", m_position );
        m_shader.setUniform( "u_winHeight", static_cast< float >( ConfigManager::getInstance().getScreenHeight() ) );
        m_shader.setUniform( "u_size", m_trackSize );
        m_shader.setUniform( "u_thumbPos", m_thumbPosition );
        m_shader.setUniform( "u_thumbSize", m_thumbSize );
        m_shader.setUniform( "u_normalizedScroll", m_normalizedScroll );
        m_shader.setUniform( "u_mouse", m_mouseLocalPos );

        window.draw( m_track, &m_shader );
    }

    bool isVisible() const
    {
        return m_contentHeight > m_viewportHeight;
    }

    float getWidth() const
    {
        float scale = ConfigManager::getInstance().getScale();
        return DEFAULT_WIDTH * scale;
    }

private:
    void updateThumbGeometry()
    {
        float scale = ConfigManager::getInstance().getScale();

        if ( m_contentHeight <= m_viewportHeight || m_contentHeight <= 0.0f || m_viewportHeight <= 0.0f )
        {
            m_thumbHeight = m_trackHeight;
            m_thumbSize = { m_trackSize.x, m_thumbHeight };
            // Thumb center at track center when no scrolling needed
            m_thumbPosition = m_position + sf::Vector2f( 0.0f, m_trackHeight / 2.0f );
            m_normalizedScroll = 0.0f;
            m_scrollOffset = 0.0f;
            m_scrollOffsetTarget = 0.0f;
            return;
        }

        // Calculate visible ratio - how much of content fits in viewport
        float visibleRatio = m_viewportHeight / m_contentHeight;
        visibleRatio = std::clamp( visibleRatio, 0.1f, 1.0f );

        // Thumb height is proportional to visible ratio - stays constant during scroll
        m_thumbHeight = std::max( m_trackHeight * visibleRatio, MIN_THUMB_HEIGHT * scale );
        m_thumbHeight = std::min( m_thumbHeight, m_trackHeight );

        m_thumbSize = { m_trackSize.x, m_thumbHeight };

        // ALWAYS ensure thumb is at top when scrollOffset is 0
        if ( m_scrollOffset <= 0.0f )
        {
            m_normalizedScroll = 0.0f;
            // Thumb center at top of track (accounting for thumb half-height)
            m_thumbPosition = m_position + sf::Vector2f( 0.0f, m_thumbHeight / 2.0f );
            return;
        }

        // Calculate thumb position based on scroll offset
        float contentScrollableDistance = m_contentHeight - m_viewportHeight;
        
        if ( contentScrollableDistance > 1.0f )
        {
            m_normalizedScroll = m_scrollOffset / contentScrollableDistance;
            float thumbScrollableDistance = m_trackHeight - m_thumbHeight;
            // Thumb position is its CENTER, so add half thumb height
            float thumbY = m_position.y + ( m_normalizedScroll * thumbScrollableDistance ) + ( m_thumbHeight / 2.0f );
            m_thumbPosition = { m_position.x, std::clamp( thumbY, m_position.y + ( m_thumbHeight / 2.0f ), m_position.y + m_trackHeight - ( m_thumbHeight / 2.0f ) ) };
        }
        else
        {
            m_normalizedScroll = 0.0f;
            m_thumbPosition = m_position + sf::Vector2f( 0.0f, m_thumbHeight / 2.0f );
        }
    }

    bool isPointInsideTrack( sf::Vector2f point ) const
    {
        float halfW = m_trackSize.x / 2.0f;
        float pX = point.x - m_position.x;
        float pY = point.y - m_position.y;

        return std::abs( pX ) <= halfW && pY >= 0.0f && pY <= m_trackHeight;
    }

    bool isPointInsideThumb( sf::Vector2f point ) const
    {
        float halfW = m_thumbSize.x / 2.0f;
        float halfH = m_thumbSize.y / 2.0f;

        float pX = point.x - m_thumbPosition.x;
        float pY = point.y - m_thumbPosition.y;

        return std::abs( pX ) <= halfW && std::abs( pY ) <= halfH;
    }

    std::shared_ptr< sf::Font > m_font;
    sf::RectangleShape m_track;
    sf::Vector2f m_trackSize;
    sf::Vector2f m_position;
    sf::Vector2f m_thumbSize;
    sf::Vector2f m_thumbPosition;
    sf::Shader m_shader;
    sf::Clock m_clock;
    float m_scale{ 1.0f };
    float m_dragOffset{ 0.0f };  // Offset between mouse and thumb center when dragging

    float m_trackHeight;
    float m_contentHeight{ 0.0f };
    float m_viewportHeight{ 0.0f };
    float m_thumbHeight{ 0.0f };
    float m_scrollOffset{ 0.0f };
    float m_scrollOffsetTarget{ 0.0f };
    float m_normalizedScroll{ 0.0f };

    bool m_isHovered{ false };
    bool m_isThumbHovered{ false };
    bool m_isDragging{ false };
    sf::Vector2f m_mouseLocalPos;
};
