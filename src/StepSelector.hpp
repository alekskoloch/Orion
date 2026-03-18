#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "ConfigManager.hpp"
#include "InputContext.hpp"
#include "Window.hpp"

class StepSelector
{
public:
    using Callback = std::function< void( int ) >;

    static constexpr float DEFAULT_WIDTH = 600.f;
    static constexpr float DEFAULT_HEIGHT = 32.f;
    static constexpr float HANDLE_SIZE = 28.f;    // Square handle size

    /// Create a step selector with custom step labels
    /// @param position Screen position
    /// @param callback Callback receiving current step index
    /// @param steps Vector of step labels (e.g. {"LOW", "MID", "HIGH"} or {"1920x1080", "2560x1440"})
    /// @param initialStep Initial step index (0-based)
    StepSelector( sf::Vector2f position, Callback callback, std::vector< std::string > steps, int initialStep = 0 )
        : m_callback( std::move( callback ) ), m_font( std::make_shared< sf::Font >() ), m_valueText( *m_font ),
          m_steps( std::move( steps ) ), m_currentStep( std::clamp( initialStep, 0, static_cast< int >( m_steps.size() - 1 ) ) ),
          m_targetStep( m_currentStep ), m_releaseTime( -100.0f )
    {
        float scale = ConfigManager::getInstance().getScale();

        m_canvasSize = { DEFAULT_WIDTH * scale, DEFAULT_HEIGHT * scale };
        m_canvas.setSize( m_canvasSize );
        m_canvas.setOrigin( m_canvasSize / 2.f );
        m_canvas.setPosition( position );
        m_canvas.setFillColor( sf::Color::White );

        std::string shaderPath = std::string( ASSETS_PATH ) + "shaders/step_selector.frag";
        if ( !m_shader.loadFromFile( shaderPath, sf::Shader::Type::Fragment ) )
        {
            std::cerr << "CRITICAL: StepSelector shader load failed: " << shaderPath << std::endl;
        }

        std::string fontPath = std::string( ASSETS_PATH ) + "fonts/ScienceGothic-Regular.ttf";
        if ( !m_font->openFromFile( fontPath ) )
        {
            std::cerr << "ERROR: Font load failed: " << fontPath << std::endl;
        }

        m_valueText.setCharacterSize( static_cast< unsigned int >( 38 * scale ) );
        m_valueText.setFillColor( sf::Color( 200, 200, 220 ) );

        m_valueTextOffset = { ( DEFAULT_WIDTH * scale / 2.f ) + ( 35.f * scale ), -( 25.f * scale ) };

        updateValueTextGeometry();
        invokeCallback();
    }

    /// Create a simple on/off step selector
    /// @param position Screen position
    /// @param callback Callback receiving current step index (0=off, 1=on)
    /// @param isOn Initial state
    StepSelector( sf::Vector2f position, Callback callback, bool isOn = false )
        : StepSelector( position, std::move( callback ), std::vector< std::string >{ "OFF", "ON" }, isOn ? 1 : 0 )
    {
    }

    void handleEvent( const InputContext& inputContext )
    {
        const auto& event = inputContext.getEvent();
        const auto& mouseState = inputContext.getMouseState();
        sf::Vector2f mousePos{ mouseState.worldPosition.x, mouseState.worldPosition.y };

        bool isHovered = isPointInside( mousePos );
        m_isHovered = isHovered;

        if ( const auto* pressed = event.getIf< sf::Event::MouseButtonPressed >() )
        {
            if ( pressed->button == sf::Mouse::Button::Left && isHovered )
            {
                // Immediately snap to nearest step on click
                snapToNearestStep();
                m_isDragging = true;
            }
        }

        if ( m_isDragging )
        {
            // While dragging, continuously update step based on mouse position
            snapToNearestStep();

            if ( const auto* released = event.getIf< sf::Event::MouseButtonReleased >() )
            {
                if ( released->button == sf::Mouse::Button::Left )
                {
                    m_isDragging = false;
                    m_releaseTime = m_clock.getElapsedTime().asSeconds(); // Trigger sweep on release
                }
            }
        }

        m_mouseLocalPos = mousePos - m_canvas.getPosition();
    }

    void update( sf::Time deltaTime )
    {
        // Instant snap to target (no interpolation for segmented design)
        m_currentStep = static_cast< float >( m_targetStep );

        m_shader.setUniform( "u_time", m_clock.getElapsedTime().asSeconds() );
        m_shader.setUniform( "u_hover", m_isHovered ? 1.0f : 0.0f );
        m_shader.setUniform( "u_dragging", m_isDragging ? 1.0f : 0.0f );
        m_shader.setUniform( "u_releaseTime", m_releaseTime );

        // Update normalized value for shader (0.0 to 1.0)
        int numSteps = static_cast< int >( m_steps.size() );
        m_normalizedValue = ( numSteps > 1 ) ? ( m_currentStep / static_cast< float >( numSteps - 1 ) ) : 0.0f;

        // Update displayed value text (current step label)
        std::string valueStr = m_steps[ m_targetStep ];
        m_valueText.setString( valueStr );
    }

    void setStep( int step )
    {
        m_targetStep = std::clamp( step, 0, static_cast< int >( m_steps.size() - 1 ) );
        m_currentStep = static_cast< float >( m_targetStep );
        invokeCallback();
    }

    int getStep() const { return m_targetStep; }
    std::string getStepLabel() const { return m_steps[ static_cast< int >( std::round( m_currentStep ) ) ]; }
    int getStepCount() const { return static_cast< int >( m_steps.size() ); }

    sf::Vector2f getPosition() const { return m_canvas.getPosition(); }
    void setPosition( const sf::Vector2f& pos )
    {
        m_canvas.setPosition( pos );
        updateValueTextGeometry();
    }
    sf::Vector2f getSize() const { return m_canvasSize; }

    void draw( Window& window )
    {
        m_shader.setUniform( "u_pos", m_canvas.getPosition() );
        m_shader.setUniform( "u_winHeight", static_cast< float >( ConfigManager::getInstance().getScreenHeight() ) );
        m_shader.setUniform( "u_size", sf::Vector2f( m_canvasSize.x, m_canvasSize.y ) );
        m_shader.setUniform( "u_value", m_normalizedValue );
        m_shader.setUniform( "u_mouse", m_mouseLocalPos );
        m_shader.setUniform( "u_stepCount", static_cast< float >( m_steps.size() ) );

        window.draw( m_canvas, &m_shader );
        window.draw( m_valueText );
    }

private:
    bool isPointInside( sf::Vector2f point ) const
    {
        sf::Vector2f center = m_canvas.getPosition();
        float halfW = m_canvasSize.x / 2.f;
        float halfH = m_canvasSize.y / 2.f;

        float dX = point.x - center.x;
        float dY = point.y - center.y;

        return std::abs( dX ) <= halfW && std::abs( dY ) <= halfH;
    }

    void snapToNearestStep()
    {
        float scale = ConfigManager::getInstance().getScale();
        float trackHalfWidth = ( DEFAULT_WIDTH * scale * 0.7f ) * 0.5f;
        float trackLeftX = m_canvas.getPosition().x - trackHalfWidth;
        float trackRightX = m_canvas.getPosition().x + trackHalfWidth;

        float clampedX = std::clamp( m_mouseLocalPos.x + m_canvas.getPosition().x, trackLeftX, trackRightX );
        float t = ( clampedX - trackLeftX ) / ( trackRightX - trackLeftX );

        int numSteps = static_cast< int >( m_steps.size() );
        if ( numSteps > 1 )
        {
            float stepWidth = 1.0f / static_cast< float >( numSteps - 1 );
            int nearestStep = static_cast< int >( std::round( t / stepWidth ) );
            m_targetStep = std::clamp( nearestStep, 0, numSteps - 1 );
        }
        else
        {
            m_targetStep = 0;
        }

        invokeCallback();
    }

    void updateValueTextGeometry()
    {
        sf::Vector2f valuePos = m_canvas.getPosition() + m_valueTextOffset;
        m_valueText.setPosition( valuePos );
    }

    void invokeCallback()
    {
        if ( m_callback )
        {
            m_callback( m_targetStep );
        }
    }

    Callback m_callback;
    sf::RectangleShape m_canvas;
    sf::Vector2f m_canvasSize;
    sf::Vector2f m_valueTextOffset;
    sf::Shader m_shader;
    sf::Clock m_clock;
    std::shared_ptr< sf::Font > m_font;
    sf::Text m_valueText;
    std::vector< std::string > m_steps;
    float m_currentStep;     // Smooth interpolated value
    int m_targetStep;        // Actual selected step
    float m_normalizedValue; // 0.0 to 1.0 for shader
    bool m_isHovered = false;
    bool m_isDragging = false;
    sf::Vector2f m_mouseLocalPos;
    float m_releaseTime;
};
