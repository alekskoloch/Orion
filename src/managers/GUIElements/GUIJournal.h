#pragma once

#include "../../pch.h"

#include "../../managers/FontManager.h"

#include "../../systems/QuestCore/Quest.h"

#include "GUIButton.h"

namespace journal
{
    float const WINDOW_POSITION_X_PERCENTAGE = 0.5f;
    float const WINDOW_POSITION_Y_PERCENTAGE = 0.5f;

    float const WINDOW_WIDTH_PERCENTAGE = 0.5f;
    float const WINDOW_HEIGHT_PERCENTAGE = 0.5f;

    float const BOOKMARK_BAR_HEIGHT_PERCENTAGE = 0.1f;
    float const SELECT_BOX_WIDTH_PERCENTAGE = 0.2f;
    float const CONTENT_BOX_WIDTH_PERCENTAGE = 0.8f;

    float const MARGIN = 20.0f;

    sf::Color const DEFAULT_BOX_COLOR = sf::Color(0, 0, 0, 200);
    sf::Color const DEFAULT_OUTLINE_COLOR = sf::Color::White;
    float const DEFAULT_OUTLINE_THICKNESS = 2.0f;

    std::string const TITLE_TEXT = "Journal";
    unsigned int const CHARACTER_SIZE = 40;
    unsigned int const SMALL_CHARACTER_SIZE = 20;
    sf::Color const DEFAULT_TEXT_COLOR = sf::Color::White;
    sf::Color const POSITIVE_TEXT_COLOR = sf::Color::Green;

    float const BUTTON_HEIGHT = 50.0f;
    sf::Color const BUTTON_COLOR = sf::Color(0, 0, 0, 200);
    sf::Color const BUTTON_HOVER_COLOR = sf::Color(0, 100, 0, 200);
    sf::Color const BUTTON_ACTIVE_COLOR = sf::Color(0, 200, 0, 200);
}

class GUIJournal : public sf::Drawable
{

public:
    GUIJournal(sf::RenderWindow& window, entt::registry& registry, std::vector<Quest>& quests);
    
    bool isOpened() const { return this->isOpen; }

    void processInput(sf::Event& event);
    void update(sf::Time deltaTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    void initializeGUIJournal();
    void initializeGUIJournalSize();
    void initializeGUIJournalPosition();

    void initializeBoxes();
    void initializeBookmarkBar();
    void initializeContentBox();
    void initializeSelectBox();
    void initializeJournalBoxElement(sf::RectangleShape& element);

    void initializeTitleText();
    sf::Text getJournalStyleText(const std::string text, const unsigned int characterSize = journal::CHARACTER_SIZE, const sf::Color color = journal::DEFAULT_TEXT_COLOR);

    void initializeMaxVisibleButtons();
    unsigned int calculateMaxVisibleButtons();

    void initializeButtons();
    void initializeSortButtons();
    GUIButton getJournalButtonStyle(const std::string text, const sf::Vector2f position, const sf::Vector2f size, ButtonStyle style = ButtonStyle::Borderless);

    void sortAndDisplayQuests();
    void setButtons();
    void setContentText();
    void setAllQuestsInactive();

    sf::RenderWindow& window;
    entt::registry& registry;

    bool isOpen = false;
    bool isButtonReleased = false;
    bool isMouseReleased = false;

    std::vector<Quest>& quests;
    std::vector<std::reference_wrapper<Quest>> sortedQuests;

    sf::Vector2f position;
    sf::Vector2f size;

    //TODO: Refactor this to a GUIBox class
    sf::RectangleShape contentBox;
    sf::RectangleShape bookmarkBar;
    sf::RectangleShape selectBox;
    sf::Font font;
    
    sf::Text titleText;
    std::vector<sf::Text> contentText;

    std::vector<GUIButton> buttons;
    std::vector<GUIButton> sortButtons;
    enum class SortType
    {
        All,
        Current,
        Completed,
        SORT_TYPE_COUNT
    };
    SortType sortType = SortType::All;

    int scrollPosition = 0;
    int maxVisibleButtons;
};