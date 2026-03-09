#include "TabWindow.hpp"
#include "ConfigManager.hpp"
#include "Window.hpp"
#include <iostream>
#include <cmath>

namespace
{
    constexpr float BASE_TAB_WIDTH  = 450.0f; 
    constexpr float BASE_TAB_HEIGHT = 95.0f;  
    constexpr float TAB_SKEW_AMOUNT = 60.0f;

    constexpr float TAB_OVERLAP = 25.0f; 
    constexpr float TAB_SPACING_OFFSET = -TAB_OVERLAP; 

    constexpr float BODY_WIDTH_RATIO  = 0.95f;
    constexpr float BODY_HEIGHT_RATIO = 0.75f;
    constexpr float BODY_POS_Y_RATIO  = 0.60f;
}

TabWindow::TabWindow()
    : m_font( std::make_shared< sf::Font >() )
    , m_bodyShader( std::make_shared< sf::Shader >() )
    , m_tabShader( std::make_shared< sf::Shader >() )
{
    initGeometry();
}

void TabWindow::initGeometry()
{
    const auto& config = ConfigManager::getInstance();
    m_scale = config.getScale();

    // ZMIANA: Ładujemy ten sam plik shadera dla obu komponentów
    std::string shaderPath = std::string( ASSETS_PATH ) + "shaders/tab_unified.frag";

    if (!m_bodyShader->loadFromFile( shaderPath, sf::Shader::Type::Fragment ))
         std::cerr << "CRITICAL: Shader body error" << std::endl;
    if (!m_tabShader->loadFromFile( shaderPath, sf::Shader::Type::Fragment ))
         std::cerr << "CRITICAL: Shader item error" << std::endl;
         
    m_font->openFromFile( std::string( ASSETS_PATH ) + "fonts/ScienceGothic-Regular.ttf" );

    float screenW = static_cast< float >( config.getScreenWidth() );
    float screenH = static_cast< float >( config.getScreenHeight() );

    m_bodySize = { screenW * BODY_WIDTH_RATIO, screenH * BODY_HEIGHT_RATIO };
    m_bodyPosition = { screenW * 0.5f, screenH * BODY_POS_Y_RATIO };

    m_bodyCanvas.setSize( m_bodySize );
    m_bodyCanvas.setOrigin( m_bodySize / 2.0f );
    m_bodyCanvas.setPosition( m_bodyPosition );
    m_bodyCanvas.setFillColor( sf::Color::Black );

    const std::vector< std::string > names = { "General", "Graphics", "Audio", "Controls" };

    float tabWidth = BASE_TAB_WIDTH * m_scale;
    float tabHeight = BASE_TAB_HEIGHT * m_scale;
    float skew = TAB_SKEW_AMOUNT * m_scale;
    float tabSpacing = TAB_SPACING_OFFSET * m_scale;

    float verticalOverlap = 4.0f * m_scale; 
    float bodyTopY = m_bodyPosition.y - (m_bodySize.y / 2.0f);
    float tabPosY = bodyTopY - (tabHeight / 2.0f) + verticalOverlap; 

    float stepX = tabWidth + tabSpacing;
    float bodyLeftX = m_bodyPosition.x - (m_bodySize.x / 2.0f);
    float startX = bodyLeftX + (tabWidth / 2.0f) + (20.0f * m_scale); 

    for ( size_t i = 0; i < names.size(); ++i )
    {
        TabElement tab( *m_font );
        tab.title = names[i];
        tab.id = static_cast< SettingsTab >( i );

        tab.text.setString( names[i] );
        tab.text.setCharacterSize( static_cast< unsigned int >( 42 * m_scale ) );
        tab.text.setStyle( sf::Text::Bold );

        tab.shape.setSize( { tabWidth, tabHeight } );
        tab.shape.setOrigin( { tabWidth / 2.0f, tabHeight / 2.0f } );

        float currentX = startX + ( static_cast<float>(i) * stepX );
        tab.shape.setPosition( { currentX, tabPosY } );

        tab.boundingBox = tab.shape.getGlobalBounds(); 
        tab.skewOffset = skew;

        sf::Text refText( *m_font, "A", tab.text.getCharacterSize() );
        refText.setStyle( sf::Text::Bold );
        auto refBounds = refText.getLocalBounds();
        
        float originY = refBounds.position.y + refBounds.size.y / 2.0f;
        auto textBounds = tab.text.getLocalBounds();
        float originX = textBounds.position.x + textBounds.size.x / 2.0f;

        tab.text.setOrigin( { originX, originY } );
        tab.text.setPosition( { currentX, tabPosY - (verticalOverlap * 0.5f) } );

        m_tabs.push_back( std::move( tab ) );
    }
}

bool TabWindow::isPointInsideTab(const sf::Vector2f& point, const TabElement& tab)
{
    float left = tab.boundingBox.position.x;
    float top = tab.boundingBox.position.y;
    float width = tab.boundingBox.size.x;
    float height = tab.boundingBox.size.y;

    float localX = point.x - left;
    float localY = point.y - top;

    if (localX < 0 || localX > width || localY < 0 || localY > height) return false;

    float slopeFactor = localY / height;
    float xLeftLimit = tab.skewOffset * (1.0f - slopeFactor);
    float xRightLimit = width - (tab.skewOffset * (1.0f - slopeFactor));

    return (localX >= xLeftLimit && localX <= xRightLimit);
}

void TabWindow::handleEvent( const InputContext& inputContext )
{
    const auto& mouseState = inputContext.getMouseState();
    sf::Vector2f mousePos{ (float)mouseState.worldPosition.x, (float)mouseState.worldPosition.y };
    
    bool clicked = false;
    if ( const auto* pressed = inputContext.getEvent().getIf< sf::Event::MouseButtonPressed >() )
        if ( pressed->button == sf::Mouse::Button::Left ) clicked = true;

    for(auto& tab : m_tabs) tab.isHovered = false;

    for ( auto it = m_tabs.rbegin(); it != m_tabs.rend(); ++it )
    {
        if ( isPointInsideTab(mousePos, *it) )
        {
            it->isHovered = true;
            if ( clicked ) 
            {
                m_activeTab = it->id;
                m_timeSinceClick = 0.0f;
                m_clickPos = mousePos;
            }
            break; 
        }
    }
}

void TabWindow::update( sf::Time deltaTime ) 
{
    m_timeSinceClick += deltaTime.asSeconds();
}

void TabWindow::draw( Window& window )
{
    float screenH = static_cast< float >( ConfigManager::getInstance().getScreenHeight() );
    sf::Vector2f bodyTopLeft = m_bodyCanvas.getPosition() - m_bodyCanvas.getOrigin();

    if ( m_bodyShader ) {
        // --- ZMIANA: USTAWIENIE TYPU NA 0 (BODY) ---
        m_bodyShader->setUniform( "u_type", 0 );
        
        m_bodyShader->setUniform( "u_pos", bodyTopLeft );
        m_bodyShader->setUniform( "u_winHeight", screenH );
        m_bodyShader->setUniform( "u_size", m_bodySize );
        
        // Przekazujemy clickPos i clickTime normalnie
        m_bodyShader->setUniform( "u_clickPos", m_clickPos );
        m_bodyShader->setUniform( "u_clickTime", m_timeSinceClick );

        // Pozostałe zmienne (u_skew, u_active itp) nie są potrzebne dla body (type 0), 
        // ale shader je ma zdefiniowane, więc nie musimy ich wysyłać (będą 0).

        window.draw( m_bodyCanvas, m_bodyShader.get() );
    } else window.draw( m_bodyCanvas );

    auto drawTab = [&]( const TabElement& tab, bool forceActive ) {
        if ( m_tabShader ) {
            // --- ZMIANA: USTAWIENIE TYPU NA 1 (TAB) ---
            m_tabShader->setUniform( "u_type", 1 );

            sf::Vector2f topLeft = tab.shape.getPosition() - tab.shape.getOrigin();
            
            m_tabShader->setUniform( "u_pos", topLeft );
            m_tabShader->setUniform( "u_size", sf::Vector2f(tab.boundingBox.size.x, tab.boundingBox.size.y) );
            m_tabShader->setUniform( "u_skew", tab.skewOffset );
            m_tabShader->setUniform( "u_winHeight", screenH );
            m_tabShader->setUniform( "u_active", forceActive );
            m_tabShader->setUniform( "u_hover", tab.isHovered );
            m_tabShader->setUniform( "u_bodyPosX", bodyTopLeft.x );
            m_tabShader->setUniform( "u_bodyWidth", m_bodySize.x );

            m_tabShader->setUniform( "u_clickPos", m_clickPos );
            m_tabShader->setUniform( "u_clickTime", m_timeSinceClick );
            
            window.draw( tab.shape, m_tabShader.get() );
        } else window.draw( tab.shape );
        
        sf::Text tempText = tab.text;
        if(forceActive) tempText.setFillColor( sf::Color::White );
        else tempText.setFillColor( sf::Color(140, 140, 160) );
        window.draw( tempText );
    };

    for ( const auto& tab : m_tabs ) {
        if ( tab.id != m_activeTab ) drawTab( tab, false );
    }
    for ( const auto& tab : m_tabs ) {
        if ( tab.id == m_activeTab ) drawTab( tab, true );
    }
}