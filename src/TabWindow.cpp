#include "TabWindow.hpp"
#include "ConfigManager.hpp"
#include "Window.hpp"

#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iterator>

// OpenGL headers for scissor test
#include <SFML/OpenGL.hpp>

namespace
{
    namespace Geometry
    {
        constexpr float BaseTabWidth = 450.0f;
        constexpr float BaseTabHeight = 95.0f;
        constexpr float TabSkewAmount = 60.0f;
        constexpr float TabOverlap = 25.0f;
        constexpr float TabSpacingOffset = -TabOverlap;
        constexpr float VerticalOverlapCorrection = 5.0f;
        constexpr float TextSize = 42.0f;
        constexpr float StartXOffset = 20.0f;
    }

    namespace ShaderUniforms
    {
        const char* Type = "u_type";
        const char* Pos = "u_pos";
        const char* Size = "u_size";
        const char* WinHeight = "u_winHeight";
        const char* ClickPos = "u_clickPos";
        const char* ClickTime = "u_clickTime";
        const char* Skew = "u_skew";
        const char* Active = "u_active";
        const char* Hover = "u_hover";
        const char* BodyPosX = "u_bodyPosX";
        const char* BodyWidth = "u_bodyWidth";
    }

    namespace Assets
    {
        const char* ShaderPath = "shaders/tab_unified.frag";
        const char* FontPath = "fonts/ScienceGothic-Regular.ttf";
    }

    namespace Colors
    {
        const sf::Color BodyFill = sf::Color::Black;
        const sf::Color TextActive = sf::Color::White;
        const sf::Color TextInactive = sf::Color( 140, 140, 160 );
    }

    constexpr int ShaderTypeBody = 0;
    constexpr int ShaderTypeTab = 1;
}

TabWindow::TabWindow( const sf::Vector2f& size, const sf::Vector2f& position )
    : m_size( size )
    , m_position( position )
    , m_shader( std::make_shared< sf::Shader >() )
    , m_font( std::make_shared< sf::Font >() )
{
    const auto& config = ConfigManager::getInstance();
    m_scale = config.getScale();

    std::string fullShaderPath = std::string( ASSETS_PATH ) + Assets::ShaderPath;
    if ( !m_shader->loadFromFile( fullShaderPath, sf::Shader::Type::Fragment ) )
        std::cerr << "CRITICAL: Failed to load tab shader from " << fullShaderPath << std::endl;

    std::string fullFontPath = std::string( ASSETS_PATH ) + Assets::FontPath;
    if ( !m_font->openFromFile( fullFontPath ) )
        std::cerr << "CRITICAL: Failed to load font from " << fullFontPath << std::endl;

    m_bodyCanvas.setSize( m_size );
    m_bodyCanvas.setOrigin( m_size / 2.0f );
    m_bodyCanvas.setPosition( m_position );
    m_bodyCanvas.setFillColor( Colors::BodyFill );

    // Initialize scrollbar
    float scrollbarX = m_position.x + ( m_size.x / 2.0f ) - ( 20.0f * m_scale );
    float scrollbarY = m_position.y - ( m_size.y / 2.0f ) + ( 10.0f * m_scale );
    float scrollbarHeight = m_size.y - ( 20.0f * m_scale );
    
    m_scrollbar = std::make_unique< Scrollbar >( sf::Vector2f( scrollbarX, scrollbarY ), scrollbarHeight );
}

TabWindow& TabWindow::addTab( SettingsTab id, const std::string& title, std::vector< TabChildElement >&& content )
{
    TabData newTab{ id, title, sf::RectangleShape(), sf::Text( *m_font ), sf::FloatRect(), 0.0f, false, std::move( content ), 0.0f };

    newTab.text.setString( title );
    newTab.text.setCharacterSize( static_cast< unsigned int >( Geometry::TextSize * m_scale ) );
    newTab.text.setStyle( sf::Text::Bold );

    // Calculate content height based on SettingSections
    // Count sections and calculate total height needed
    float minY = std::numeric_limits< float >::max();
    float maxY = std::numeric_limits< float >::lowest();
    float sectionHeight = 100.0f * m_scale;  // Default section height
    int sectionCount = 0;

    for ( const auto& element : newTab.content )
    {
        std::visit( [ &minY, &maxY, &sectionHeight, &sectionCount ]( const auto& widget )
        {
            if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( widget ) >, SettingSection > )
            {
                float elemY = widget.getBasePosition().y;
                minY = std::min( minY, elemY );
                maxY = std::max( maxY, elemY );
                sectionHeight = std::max( sectionHeight, widget.getHeight() );
                sectionCount++;
            }
        }, element );
    }

    // Store where content starts (first element Y position)
    if ( sectionCount > 0 )
    {
        newTab.contentStartY = minY;
    }
    else
    {
        newTab.contentStartY = 0.0f;
    }

    // Total content height = distance from first to last section + height of one section
    // But we need to account for content that starts ABOVE the viewport
    // Viewport top will be calculated in updateScrollbar
    if ( sectionCount > 1 && maxY > minY )
    {
        newTab.contentHeight = ( maxY - minY ) + sectionHeight;
    }
    else if ( sectionCount > 0 )
    {
        // Only one section or all at same Y
        newTab.contentHeight = sectionHeight * static_cast< float >( sectionCount );
    }
    else
    {
        // No sections - estimate based on total elements
        newTab.contentHeight = static_cast< float >( newTab.content.size() ) * 100.0f * m_scale;
    }

    m_tabs.push_back( std::move( newTab ) );

    updateTabGeometry();

    // Update scrollbar for the newly added tab
    updateScrollbar();

    return *this;
}

void TabWindow::updateScrollbar()
{
    if ( !m_scrollbar ) return;

    auto activeTab = std::find_if( m_tabs.begin(), m_tabs.end(),
        [ this ]( const TabData& tab ) { return tab.id == m_activeTab; } );

    if ( activeTab != m_tabs.end() )
    {
        // Available viewport height for content (body height minus padding)
        float viewportHeight = m_size.y - ( 40.0f * m_scale );
        
        // Viewport top in world coordinates
        float viewportTop = m_position.y - ( m_size.y / 2.0f ) + ( 20.0f * m_scale );
        
        // Content bottom in world coordinates
        float contentBottom = activeTab->contentStartY + activeTab->contentHeight;
        
        // Effective content height is from viewport top to content bottom
        // This ensures scrollbar thumb position matches visible content position
        float effectiveContentHeight = contentBottom - viewportTop;
        
        // Ensure content height is at least the viewport height
        effectiveContentHeight = std::max( effectiveContentHeight, viewportHeight );
        
        // Force reset scroll to top when updating scrollbar
        m_scrollbar->resetScrollToTop();
        m_scrollbar->setContentHeight( effectiveContentHeight, viewportHeight );
    }
}

void TabWindow::updateTabGeometry()
{
    if ( m_tabs.empty() ) return;

    float tabWidth = Geometry::BaseTabWidth * m_scale;
    float tabHeight = Geometry::BaseTabHeight * m_scale;
    float skew = Geometry::TabSkewAmount * m_scale;
    float tabSpacing = Geometry::TabSpacingOffset * m_scale;
    float verticalOverlap = Geometry::VerticalOverlapCorrection * m_scale;
    
    float bodyTopY = m_position.y - ( m_size.y / 2.0f );
    float tabPosY = bodyTopY - ( tabHeight / 2.0f ) + verticalOverlap;
    
    float stepX = tabWidth + tabSpacing;
    float bodyLeftX = m_position.x - ( m_size.x / 2.0f );
    float startX = bodyLeftX + ( tabWidth / 2.0f ) + ( Geometry::StartXOffset * m_scale );

    sf::Text refText( *m_font, "A", static_cast< unsigned int >( Geometry::TextSize * m_scale ) );
    refText.setStyle( sf::Text::Bold );
    auto refBounds = refText.getLocalBounds();
    float textOriginY = refBounds.position.y + refBounds.size.y / 2.0f;

    for ( size_t i = 0; i < m_tabs.size(); ++i )
    {
        auto& tab = m_tabs[i];
        
        tab.shape.setSize( { tabWidth, tabHeight } );
        tab.shape.setOrigin( { tabWidth / 2.0f, tabHeight / 2.0f } );
        
        float currentX = startX + ( static_cast< float >( i ) * stepX );
        tab.shape.setPosition( { currentX, tabPosY } );
        
        tab.boundingBox = tab.shape.getGlobalBounds();
        tab.skewOffset = skew;

        auto textBounds = tab.text.getLocalBounds();
        float textOriginX = textBounds.position.x + textBounds.size.x / 2.0f;

        tab.text.setOrigin( { textOriginX, textOriginY } );
        tab.text.setPosition( { currentX, tabPosY - ( verticalOverlap * 0.5f ) } );
    }
}

bool TabWindow::isPointInsideTab( const sf::Vector2f& point, const TabData& tab ) const
{
    float left = tab.boundingBox.position.x;
    float top = tab.boundingBox.position.y;
    float width = tab.boundingBox.size.x;
    float height = tab.boundingBox.size.y;

    float localX = point.x - left;
    float localY = point.y - top;

    if ( localX < 0 || localX > width || localY < 0 || localY > height ) return false;

    float slopeFactor = localY / height;
    float xLeftLimit = tab.skewOffset * ( 1.0f - slopeFactor );
    float xRightLimit = width - ( tab.skewOffset * ( 1.0f - slopeFactor ) );

    return ( localX >= xLeftLimit && localX <= xRightLimit );
}

void TabWindow::handleEvent( const InputContext& inputContext )
{
    const auto& mouseState = inputContext.getMouseState();
    sf::Vector2f mousePos{ static_cast< float >( mouseState.worldPosition.x ), static_cast< float >( mouseState.worldPosition.y ) };

    bool clicked = false;
    if ( const auto* pressed = inputContext.getEvent().getIf< sf::Event::MouseButtonPressed >() )
    {
        if ( pressed->button == sf::Mouse::Button::Left ) clicked = true;
    }

    bool clickedOnHeader = false;

    for ( auto& tab : m_tabs )
    {
        tab.isHovered = false;

        if ( !clickedOnHeader && isPointInsideTab( mousePos, tab ) )
        {
            tab.isHovered = true;
            if ( clicked )
            {
                m_activeTab = tab.id;
                m_timeSinceClick = 0.0f;
                m_clickPos = mousePos;
                clickedOnHeader = true;
                // Update scrollbar when switching tabs (will reset scroll to top)
                if ( m_scrollbar )
                {
                    updateScrollbar();
                }
            }
        }
    }

    if ( !clickedOnHeader )
    {
        for ( auto& tab : m_tabs )
        {
            if ( tab.id == m_activeTab )
            {
                // Handle scrollbar first - forceScroll=true so scroll wheel works anywhere in tab
                if ( m_scrollbar )
                {
                    m_scrollbar->handleEvent( inputContext, true );
                }

                // Apply scroll offset to content elements
                float scrollOffset = m_scrollbar ? m_scrollbar->getScrollOffset() : 0.0f;

                for ( auto& element : tab.content )
                {
                    std::visit( [ scrollOffset ]( auto& widget )
                    {
                        if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( widget ) >, SettingSection > )
                        {
                            widget.setScrollOffset( scrollOffset );
                        }
                        else if constexpr ( requires { widget.getPosition(); widget.setPosition( sf::Vector2f{} ); } )
                        {
                            auto originalPos = widget.getPosition();
                            widget.setPosition( { originalPos.x, originalPos.y - scrollOffset } );
                        }
                        else if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( widget ) >, Title > )
                        {
                            auto textPos = widget.getPosition();
                            widget.setPosition( { textPos.x, textPos.y - scrollOffset } );
                        }
                    }, element );

                    std::visit( [ &inputContext ]( auto& widget )
                    {
                        if constexpr ( requires { widget.handleEvent( inputContext ); } )
                        {
                            widget.handleEvent( inputContext );
                        }
                    }, element );
                    
                    // Restore positions after handling events
                    std::visit( [ scrollOffset ]( auto& widget )
                    {
                        if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( widget ) >, SettingSection > )
                        {
                            widget.setScrollOffset( 0.0f );
                        }
                        else if constexpr ( requires { widget.getPosition(); widget.setPosition( sf::Vector2f{} ); } )
                        {
                            auto originalPos = widget.getPosition();
                            widget.setPosition( { originalPos.x, originalPos.y + scrollOffset } );
                        }
                        else if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( widget ) >, Title > )
                        {
                            auto textPos = widget.getPosition();
                            widget.setPosition( { textPos.x, textPos.y + scrollOffset } );
                        }
                    }, element );
                }
                break;
            }
        }
    }
}

void TabWindow::update( sf::Time deltaTime )
{
    m_timeSinceClick += deltaTime.asSeconds();

    // Update scrollbar animation/state
    if ( m_scrollbar )
    {
        m_scrollbar->update( deltaTime );
    }

    for ( auto& tab : m_tabs )
    {
        if ( tab.id == m_activeTab )
        {
            for ( auto& element : tab.content )
            {
                std::visit( [ deltaTime ]( auto& widget )
                {
                    if constexpr ( requires { widget.update( deltaTime ); } )
                    {
                        widget.update( deltaTime );
                    }
                }, element );
            }
            break;
        }
    }
}

void TabWindow::draw( Window& window )
{
    float screenH = static_cast< float >( ConfigManager::getInstance().getScreenHeight() );
    sf::Vector2f bodyTopLeft = m_bodyCanvas.getPosition() - m_bodyCanvas.getOrigin();

    if ( m_shader )
    {
        m_shader->setUniform( ShaderUniforms::Type, ShaderTypeBody );
        m_shader->setUniform( ShaderUniforms::Pos, bodyTopLeft );
        m_shader->setUniform( ShaderUniforms::WinHeight, screenH );
        m_shader->setUniform( ShaderUniforms::Size, m_size );
        m_shader->setUniform( ShaderUniforms::ClickPos, m_clickPos );
        m_shader->setUniform( ShaderUniforms::ClickTime, m_timeSinceClick );

        window.draw( m_bodyCanvas, m_shader.get() );
    }
    else
    {
        window.draw( m_bodyCanvas );
    }

    auto renderTab = [&]( const TabData& tab, bool forceActive )
    {
        if ( m_shader )
        {
            m_shader->setUniform( ShaderUniforms::Type, ShaderTypeTab );

            sf::Vector2f topLeft = tab.shape.getPosition() - tab.shape.getOrigin();

            m_shader->setUniform( ShaderUniforms::Pos, topLeft );
            m_shader->setUniform( ShaderUniforms::Size, sf::Vector2f( tab.boundingBox.size.x, tab.boundingBox.size.y ) );
            m_shader->setUniform( ShaderUniforms::Skew, tab.skewOffset );
            m_shader->setUniform( ShaderUniforms::WinHeight, screenH );
            m_shader->setUniform( ShaderUniforms::Active, forceActive );
            m_shader->setUniform( ShaderUniforms::Hover, tab.isHovered );
            m_shader->setUniform( ShaderUniforms::BodyPosX, bodyTopLeft.x );
            m_shader->setUniform( ShaderUniforms::BodyWidth, m_size.x );
            m_shader->setUniform( ShaderUniforms::ClickPos, m_clickPos );
            m_shader->setUniform( ShaderUniforms::ClickTime, m_timeSinceClick );

            window.draw( tab.shape, m_shader.get() );
        }
        else
        {
            window.draw( tab.shape );
        }

        sf::Text label = tab.text;
        label.setFillColor( forceActive ? Colors::TextActive : Colors::TextInactive );
        window.draw( label );
    };

    for ( const auto& tab : m_tabs )
    {
        if ( tab.id != m_activeTab ) renderTab( tab, false );
    }

    for ( auto& tab : m_tabs )
    {
        if ( tab.id == m_activeTab )
        {
            renderTab( tab, true );

            // Apply scroll offset to content
            float scrollOffset = m_scrollbar ? m_scrollbar->getScrollOffset() : 0.0f;

            // Set up OpenGL scissor test to clip content to body bounds
            // Scissor coordinates in OpenGL are from bottom-left
            // Add padding to ensure content doesn't overlap window border
            float scissorPadding = 4.0f * m_scale;  // Padding from window edge
            int scissorX = static_cast< int >( bodyTopLeft.x + scissorPadding );
            int scissorY = static_cast< int >( screenH - ( bodyTopLeft.y + m_size.y - scissorPadding ) );
            int scissorW = static_cast< int >( m_size.x - ( 2.0f * scissorPadding ) );
            int scissorH = static_cast< int >( m_size.y - ( 2.0f * scissorPadding ) );

            GLboolean scissorEnabled = glIsEnabled( GL_SCISSOR_TEST );
            if ( !scissorEnabled ) glEnable( GL_SCISSOR_TEST );
            glScissor( scissorX, scissorY, scissorW, scissorH );

            for ( auto& element : tab.content )
            {
                std::visit( [ scrollOffset ]( auto& widget )
                {
                    if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( widget ) >, SettingSection > )
                    {
                        widget.setScrollOffset( scrollOffset );
                    }
                    else if constexpr ( requires { widget.getPosition(); widget.setPosition( sf::Vector2f{} ); } )
                    {
                        // Save original position
                        auto originalPos = widget.getPosition();
                        // Apply scroll offset
                        widget.setPosition( { originalPos.x, originalPos.y - scrollOffset } );
                    }
                    else if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( widget ) >, Title > )
                    {
                        auto textPos = widget.getPosition();
                        widget.setPosition( { textPos.x, textPos.y - scrollOffset } );
                    }
                }, element );

                std::visit( [ &window ]( auto& widget )
                {
                    if constexpr ( requires { widget.draw( window ); } )
                    {
                        widget.draw( window );
                    }
                }, element );

                // Restore original positions after drawing
                std::visit( [ scrollOffset ]( auto& widget )
                {
                    if constexpr ( requires { widget.getPosition(); widget.setPosition( sf::Vector2f{} ); } )
                    {
                        auto originalPos = widget.getPosition();
                        widget.setPosition( { originalPos.x, originalPos.y + scrollOffset } );
                    }
                    else if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( widget ) >, SettingSection > )
                    {
                        widget.setScrollOffset( 0.0f );
                    }
                    else if constexpr ( std::is_same_v< std::remove_cvref_t< decltype( widget ) >, Title > )
                    {
                        auto textPos = widget.getPosition();
                        widget.setPosition( { textPos.x, textPos.y + scrollOffset } );
                    }
                }, element );
            }

            // Disable scissor test after drawing content (so scrollbar is not clipped)
            glDisable( GL_SCISSOR_TEST );

            // Draw scrollbar on top (outside clipped area)
            if ( m_scrollbar && m_scrollbar->isVisible() )
            {
                m_scrollbar->draw( window );
            }

            break;
        }
    }
}