#include "GUIJournal.h"
#include "GUIButton.h"
#include "QuestCore/Quest.h"
#include "pch.h"

#include "ConfigManager.hpp"
#include "Window.hpp"

GUIJournal::GUIJournal( std::vector< Quest >& quests )
    : quests( quests ), font( FontManager::getInstance().getFont( "font" ) ), titleText{ this->font }
{
    this->isOpen = false;
    this->sortType = SortType::All;
    this->scrollPosition = 0;
    this->maxVisibleButtons = 0;

    this->initializeGUIJournal();
}

void GUIJournal::handleEvent( const InputContext& inputContext )
{
    const auto& event = inputContext.getEvent();

    if ( const auto* keyEvent = event.getIf< sf::Event::KeyPressed >() )
    {
        if ( keyEvent->scancode == sf::Keyboard::Scan::J )
        {
            this->isOpen = !this->isOpen;

            if ( this->isOpen )
            {
                this->initializeGUIJournal();
            }
            return;
        }

        if ( this->isOpen && keyEvent->scancode == sf::Keyboard::Scan::Escape )
        {
            this->isOpen = false;
            return;
        }
    }

    if ( !this->isOpen )
        return;

    if ( this->buttons.size() > this->maxVisibleButtons && this->maxVisibleButtons > 0 )
    {
        if ( const auto* mouseWheel = event.getIf< sf::Event::MouseWheelScrolled >() )
        {
            if ( mouseWheel->delta > 0 && this->scrollPosition > 0 )
            {
                this->scrollPosition--;
            }

            else if ( mouseWheel->delta < 0 && this->scrollPosition < (int)this->buttons.size() - this->maxVisibleButtons )
            {
                this->scrollPosition++;
            }
        }
    }

    if ( this->buttons.size() > this->maxVisibleButtons && this->maxVisibleButtons > 0 )
    {
        int end = std::min( (int)this->buttons.size(), this->scrollPosition + this->maxVisibleButtons );
        for ( int i = this->scrollPosition; i < end; i++ )
        {
            this->buttons[ i ].handleEvent( inputContext );
        }
    }
    else
    {
        for ( auto& button : this->buttons )
        {
            button.handleEvent( inputContext );
        }
    }

    for ( auto& button : this->sortButtons )
    {
        button.handleEvent( inputContext );
    }
}

void GUIJournal::update( sf::Time deltaTime )
{
    if ( !this->isOpen )
        return;

    if ( this->buttons.size() > this->maxVisibleButtons && this->maxVisibleButtons > 0 )
    {
        int end = std::min( (int)this->buttons.size(), this->scrollPosition + this->maxVisibleButtons );
        for ( int i = this->scrollPosition; i < end; i++ )
        {
            float yPos = this->selectBox.getPosition().y - ( this->selectBox.getSize().y / 2.f ) +
                         ( journal::BUTTON_HEIGHT / 2.f ) + journal::DEFAULT_OUTLINE_THICKNESS +
                         ( ( i - this->scrollPosition ) * journal::BUTTON_HEIGHT );

            this->buttons[ i ].setPosition( sf::Vector2f( this->selectBox.getPosition().x, yPos ) );
            this->buttons[ i ].update( deltaTime );
        }
    }
    else
    {
        for ( auto& button : this->buttons )
        {
            button.update( deltaTime );
        }
    }

    for ( auto& button : this->sortButtons )
    {
        button.update( deltaTime );
    }
}

void GUIJournal::draw( Window& window )
{
    if ( !this->isOpen )
        return;

    window.draw( this->contentBox );
    window.draw( this->selectBox );
    window.draw( this->bookmarkBar );
    window.draw( this->titleText );

    for ( auto& text : this->contentText )
    {
        window.draw( text );
    }

    if ( this->buttons.size() > this->maxVisibleButtons && this->maxVisibleButtons > 0 )
    {
        int end = std::min( (int)this->buttons.size(), this->scrollPosition + this->maxVisibleButtons );

        for ( int i = scrollPosition; i < end; i++ )
        {
            this->buttons[ i ].draw( window );
        }
    }
    else
    {
        for ( auto& button : this->buttons )
        {
            button.draw( window );
        }
    }

    for ( auto& button : this->sortButtons )
    {
        button.draw( window );
    }
}

void GUIJournal::sortAndDisplayQuests()
{
    this->sortedQuests.clear();
    this->contentText.clear();

    if ( !this->sortButtons.empty() )
    {
        for ( auto& sortButton : this->sortButtons )
            sortButton.setDefaultState();
        this->sortButtons[ static_cast< size_t >( this->sortType ) ].setSelectedState();
    }

    switch ( sortType )
    {
    case SortType::All:
        for ( auto& quest : this->quests )
            this->sortedQuests.push_back( quest );
        break;
    case SortType::Current:
        for ( auto& quest : this->quests )
            if ( !quest.completed )
                this->sortedQuests.push_back( quest );
        break;
    case SortType::Completed:
        for ( auto& quest : this->quests )
            if ( quest.completed )
                this->sortedQuests.push_back( quest );
        break;
    default:
        for ( auto& quest : this->quests )
            this->sortedQuests.push_back( quest );
        break;
    }

    this->scrollPosition = 0;
    this->setButtons();
}

void GUIJournal::setButtons()
{
    this->buttons.clear();

    for ( size_t i = 0; i < this->sortedQuests.size(); ++i )
    {
        auto& questRef = this->sortedQuests[ i ];

        float yPos = this->selectBox.getPosition().y - this->selectBox.getSize().y / 2.f + ( journal::BUTTON_HEIGHT / 2.f ) +
                     journal::DEFAULT_OUTLINE_THICKNESS + this->buttons.size() * journal::BUTTON_HEIGHT;

        GUIButton button = this->getJournalButtonStyle(
            questRef.get().name, sf::Vector2f( this->selectBox.getPosition().x, yPos ),
            sf::Vector2f( this->selectBox.getSize().x, journal::BUTTON_HEIGHT ), ButtonStyle::Bordered );

        Quest* qPtr = &questRef.get();

        button.setOnClick(
            [ this, qPtr ]()
            {
                this->setAllQuestsInactive();
                if ( qPtr )
                    qPtr->active = true;
                this->setContentText();

                for ( auto& btn : this->buttons )
                    btn.setDefaultState();
            } );

        this->buttons.push_back( button );
    }
}

void GUIJournal::initializeSortButtons()
{
    this->sortButtons.clear();

    std::array< std::string, static_cast< size_t >( SortType::SORT_TYPE_COUNT ) > sortButtonNames = { "All", "Current",
                                                                                                      "Completed" };

    float boxWidth = ( this->selectBox.getSize().x > 0 ) ? this->selectBox.getSize().x : 100.f;

    sf::Vector2f basePosition = this->selectBox.getPosition();
    float offsetX = boxWidth / static_cast< int >( SortType::SORT_TYPE_COUNT );
    float positionY = basePosition.y + this->selectBox.getSize().y / 2.f - journal::BUTTON_HEIGHT / 2.f;

    for ( size_t i = 0; i < sortButtonNames.size(); ++i )
    {
        float positionX = basePosition.x - offsetX + offsetX * i;

        GUIButton sortButton = this->getJournalButtonStyle(
            sortButtonNames[ i ], sf::Vector2f( positionX, positionY ),
            sf::Vector2f( boxWidth / static_cast< int >( SortType::SORT_TYPE_COUNT ), journal::BUTTON_HEIGHT ),
            ButtonStyle::Bordered );

        SortType type = static_cast< SortType >( i );

        sortButton.setOnClick(
            [ this, type ]()
            {
                this->sortType = type;
                this->sortAndDisplayQuests();
            } );

        this->sortButtons.push_back( sortButton );
    }
}

void GUIJournal::initializeGUIJournal()
{
    this->initializeGUIJournalSize();
    this->initializeGUIJournalPosition();
    this->initializeBoxes();
    this->initializeTitleText();

    this->initializeMaxVisibleButtons();

    this->initializeSortButtons();

    this->sortAndDisplayQuests();
}

void GUIJournal::initializeGUIJournalSize()
{
    const auto w = ConfigManager::getInstance().getScreenWidth();
    const auto h = ConfigManager::getInstance().getScreenHeight();

    this->size = sf::Vector2f( ( w > 0 ? w : 800 ) * journal::WINDOW_WIDTH_PERCENTAGE,
                               ( h > 0 ? h : 600 ) * journal::WINDOW_HEIGHT_PERCENTAGE );
}
void GUIJournal::initializeGUIJournalPosition()
{
    const auto w = ConfigManager::getInstance().getScreenWidth();
    const auto h = ConfigManager::getInstance().getScreenHeight();
    this->position = sf::Vector2f( w * journal::WINDOW_POSITION_X_PERCENTAGE, h * journal::WINDOW_POSITION_Y_PERCENTAGE );
}
void GUIJournal::initializeBoxes()
{
    this->initializeBookmarkBar();
    this->initializeContentBox();
    this->initializeSelectBox();
}
void GUIJournal::initializeBookmarkBar()
{
    this->bookmarkBar.setSize( sf::Vector2f( this->size.x, this->size.y * journal::BOOKMARK_BAR_HEIGHT_PERCENTAGE ) );
    this->bookmarkBar.setPosition(
        sf::Vector2f{ position.x, position.y - this->size.y / 2.f - this->bookmarkBar.getSize().y / 2.f } );
    this->initializeJournalBoxElement( this->bookmarkBar );
}
void GUIJournal::initializeContentBox()
{
    this->contentBox.setSize( sf::Vector2f( this->size.x * journal::CONTENT_BOX_WIDTH_PERCENTAGE, this->size.y ) );
    this->contentBox.setPosition(
        sf::Vector2f{ position.x + this->size.x * journal::SELECT_BOX_WIDTH_PERCENTAGE / 2.f, position.y } );
    this->initializeJournalBoxElement( this->contentBox );
}
void GUIJournal::initializeSelectBox()
{
    this->selectBox.setSize( sf::Vector2f( this->size.x * journal::SELECT_BOX_WIDTH_PERCENTAGE, this->size.y ) );
    this->selectBox.setPosition(
        sf::Vector2f{ position.x - this->size.x * journal::CONTENT_BOX_WIDTH_PERCENTAGE / 2.f, position.y } );
    this->initializeJournalBoxElement( this->selectBox );
}
void GUIJournal::initializeJournalBoxElement( sf::RectangleShape& element )
{
    element.setFillColor( journal::DEFAULT_BOX_COLOR );
    element.setOutlineColor( journal::DEFAULT_OUTLINE_COLOR );
    element.setOutlineThickness( journal::DEFAULT_OUTLINE_THICKNESS );
    element.setOrigin( sf::Vector2f{ element.getSize().x / 2.f, element.getSize().y / 2.f } );
}
void GUIJournal::initializeTitleText()
{
    this->titleText = this->getJournalStyleText( journal::TITLE_TEXT );
    this->titleText.setPosition( this->bookmarkBar.getPosition() );
}
void GUIJournal::initializeMaxVisibleButtons() { this->maxVisibleButtons = this->calculateMaxVisibleButtons(); }
unsigned int GUIJournal::calculateMaxVisibleButtons()
{
    float h = this->selectBox.getSize().y;
    if ( h <= 0 )
        return 0;
    return static_cast< unsigned int >( std::floor( ( h - journal::BUTTON_HEIGHT ) / journal::BUTTON_HEIGHT ) );
}
void GUIJournal::initializeButtons() { this->initializeMaxVisibleButtons(); }
GUIButton GUIJournal::getJournalButtonStyle( const std::string text, const sf::Vector2f position, const sf::Vector2f size,
                                             ButtonStyle style )
{
    return GUIButton( position, size, text,
                      static_cast< unsigned int >( journal::SMALL_CHARACTER_SIZE * ConfigManager::getInstance().getScale() ),
                      journal::BUTTON_COLOR, journal::BUTTON_HOVER_COLOR, journal::BUTTON_ACTIVE_COLOR, style );
}
sf::Text GUIJournal::getJournalStyleText( const std::string text, const unsigned int characterSize, const sf::Color color )
{
    sf::Text journalStyleText{ this->font };
    journalStyleText.setCharacterSize( static_cast< unsigned int >( characterSize * ConfigManager::getInstance().getScale() ) );
    journalStyleText.setFillColor( color );
    journalStyleText.setString( text );
    journalStyleText.setOrigin( journalStyleText.getGlobalBounds().getCenter() );
    return journalStyleText;
}
void GUIJournal::setContentText()
{
    this->contentText.clear();
    if ( this->quests.empty() )
        return;
    for ( const auto& quest : this->quests )
    {
        if ( !quest.active )
            continue;
        sf::Text titleText = this->getJournalStyleText( quest.name );
        titleText.setPosition( sf::Vector2f{ this->contentBox.getPosition().x + this->contentBox.getSize().x / 2.f -
                                                 titleText.getGlobalBounds().size.x / 2.f - journal::MARGIN,
                                             this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f +
                                                 titleText.getGlobalBounds().size.y / 2.f + journal::MARGIN } );
        this->contentText.push_back( titleText );
        for ( int i = 0; i <= quest.currentStage; ++i )
        {
            sf::Color stageTextColor =
                ( i == quest.currentStage && !quest.completed ) ? journal::POSITIVE_TEXT_COLOR : sf::Color::White;
            sf::Text stageText =
                this->getJournalStyleText( quest.stages[ i ].description, journal::SMALL_CHARACTER_SIZE, stageTextColor );
            stageText.setPosition( { this->contentBox.getPosition().x - this->contentBox.getSize().x / 2.f +
                                         stageText.getGlobalBounds().size.x / 2.f + journal::MARGIN,
                                     this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f +
                                         stageText.getGlobalBounds().size.y / 2.f + journal::MARGIN +
                                         i * journal::BUTTON_HEIGHT } );
            this->contentText.push_back( stageText );
            if ( i == quest.currentStage )
            {
                std::string statusString = quest.completed ? "Quest completed" : quest.stages[ i ].condition->getProgress();
                sf::Color statusColor = quest.completed ? journal::POSITIVE_TEXT_COLOR : sf::Color::White;
                sf::Text statusText = this->getJournalStyleText( statusString, journal::CHARACTER_SIZE, statusColor );
                statusText.setPosition(
                    { this->contentBox.getPosition().x, this->contentBox.getPosition().y + this->contentBox.getSize().y / 2.f -
                                                            statusText.getGlobalBounds().size.y / 2.f - journal::MARGIN } );
                this->contentText.push_back( statusText );
            }
        }
    }
}
void GUIJournal::setAllQuestsInactive()
{
    for ( auto& quest : this->quests )
    {
        quest.active = false;
    }
}