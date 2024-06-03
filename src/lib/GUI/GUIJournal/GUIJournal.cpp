#include "pch.h"
#include "GUIJournal.h"

GUIJournal::GUIJournal(sf::RenderWindow& window, entt::registry& registry, std::vector<Quest>& quests) : window(window), registry(registry), quests(quests), font(FontManager::getInstance().getFont("font"))
{
    this->initializeGUIJournal();
}

void GUIJournal::processInput(sf::Event& event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) && this->isButtonReleased)
    {
        this->isOpen = !this->isOpen;
        this->isButtonReleased = false;
        this->contentText.clear();

        if (this->isOpen)
            this->sortAndDisplayQuests();
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        this->isButtonReleased = true;

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        this->isMouseReleased = true;

    if (this->isOpen)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->isButtonReleased)
        {
            this->isOpen = false;
            this->isButtonReleased = false;
        }

        if (this->buttons.size() > this->maxVisibleButtons)
        {
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0 && this->scrollPosition > 0)
                {
                    this->scrollPosition--;
                }
                else if (event.mouseWheelScroll.delta < 0 && this->scrollPosition < this->buttons.size() - this->maxVisibleButtons)
                {
                    this->scrollPosition++;
                }
            }
        }
    }
}

void GUIJournal::update(sf::Time deltaTime)
{
    auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

    if (this->buttons.size() > this->maxVisibleButtons)
    {
        for (int i = this->scrollPosition; i < this->maxVisibleButtons + this->scrollPosition; i++)
        {
            this->buttons[i].setPosition(
                sf::Vector2f(
                    this->selectBox.getPosition().x,
                    this->selectBox.getPosition().y - (this->selectBox.getSize().y / 2.f) + (journal::BUTTON_HEIGHT / 2.f) + journal::DEFAULT_OUTLINE_THICKNESS + ((i - this->scrollPosition) * journal::BUTTON_HEIGHT)
                )
            );
        }

        for (int i = scrollPosition; i < this->maxVisibleButtons + this->scrollPosition; i++)
        {
            this->buttons[i].update(mousePosition, this->isMouseReleased);
        }
    }
    else
    {
        for (auto& button : this->buttons)
        {
            button.update(mousePosition, this->isMouseReleased);
        }
    }

    for (auto& button : this->sortButtons)
    {
        button.update(mousePosition, this->isMouseReleased);
    }
}

void GUIJournal::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->contentBox);
    target.draw(this->selectBox);
    target.draw(this->bookmarkBar);
    target.draw(this->titleText);

    for (auto& text : this->contentText)
    {
        target.draw(text);
    }

    if (this->buttons.size() > this->maxVisibleButtons)
    {
        for (int i = scrollPosition; i < this->maxVisibleButtons + this->scrollPosition; i++)
        {
            target.draw(this->buttons[i]);
        }
    }
    else
    {
        for (auto& button : this->buttons)
        {
            target.draw(button);
        }
    }

    for (auto& button : this->sortButtons)
    {
        target.draw(button);
    }
}

void GUIJournal::initializeGUIJournal()
{
    this->initializeGUIJournalSize();
    this->initializeGUIJournalPosition();
    this->initializeBoxes();
    this->initializeTitleText();
    this->initializeButtons();
    
    this->sortAndDisplayQuests();
    this->setButtons();
}

void GUIJournal::initializeGUIJournalSize()
{
    this->size = sf::Vector2f(
        window.getSize().x * journal::WINDOW_WIDTH_PERCENTAGE,
        window.getSize().y * journal::WINDOW_HEIGHT_PERCENTAGE
    );
}

void GUIJournal::initializeGUIJournalPosition()
{
    this->position = sf::Vector2f(
        window.getSize().x * journal::WINDOW_POSITION_X_PERCENTAGE,
        window.getSize().y * journal::WINDOW_POSITION_Y_PERCENTAGE
    );
}

void GUIJournal::initializeBoxes()
{
    this->initializeBookmarkBar();
    this->initializeContentBox();
    this->initializeSelectBox();
}

void GUIJournal::initializeBookmarkBar()
{
    this->bookmarkBar.setSize(sf::Vector2f(this->size.x, this->size.y * journal::BOOKMARK_BAR_HEIGHT_PERCENTAGE));
    this->initializeJournalBoxElement(this->bookmarkBar);
    this->bookmarkBar.setPosition(position.x, position.y - this->size.y / 2.f - this->bookmarkBar.getSize().y / 2.f);
}

void GUIJournal::initializeContentBox()
{    
    this->contentBox.setSize(sf::Vector2f(this->size.x * journal::CONTENT_BOX_WIDTH_PERCENTAGE, this->size.y));
    this->initializeJournalBoxElement(this->contentBox);
    this->contentBox.setPosition(position.x + this->size.x * journal::SELECT_BOX_WIDTH_PERCENTAGE / 2.f, position.y);
}

void GUIJournal::initializeSelectBox()
{
    this->selectBox.setSize(sf::Vector2f(this->size.x * journal::SELECT_BOX_WIDTH_PERCENTAGE, this->size.y));
    this->initializeJournalBoxElement(this->selectBox);
    this->selectBox.setPosition(position.x - this->size.x * journal::CONTENT_BOX_WIDTH_PERCENTAGE / 2.f, position.y);
}

void GUIJournal::initializeJournalBoxElement(sf::RectangleShape& element)
{
    element.setFillColor(journal::DEFAULT_BOX_COLOR);
    element.setOutlineColor(journal::DEFAULT_OUTLINE_COLOR);
    element.setOutlineThickness(journal::DEFAULT_OUTLINE_THICKNESS);
    element.setOrigin(element.getSize().x / 2.f, element.getSize().y / 2.f);
}

void GUIJournal::initializeTitleText()
{
    this->titleText = this->getJournalStyleText(journal::TITLE_TEXT);
    this->titleText.setPosition(this->bookmarkBar.getPosition());
}

sf::Text GUIJournal::getJournalStyleText(const std::string text, const unsigned int characterSize, const sf::Color color)
{
    sf::Text journalStyleText;
    journalStyleText.setFont(this->font);
    journalStyleText.setCharacterSize(characterSize * ConfigManager::getInstance().getScale());
    journalStyleText.setFillColor(color);
    journalStyleText.setString(text);
    journalStyleText.setOrigin(journalStyleText.getGlobalBounds().width / 2.f, journalStyleText.getGlobalBounds().height / 2.f);
    return journalStyleText;
}

void GUIJournal::initializeMaxVisibleButtons()
{
    this->maxVisibleButtons = this->calculateMaxVisibleButtons();
}

unsigned int GUIJournal::calculateMaxVisibleButtons()
{
    return std::floor((this->selectBox.getSize().y - journal::BUTTON_HEIGHT) / journal::BUTTON_HEIGHT);
}

void GUIJournal::initializeButtons()
{
    this->initializeMaxVisibleButtons();
    this->initializeSortButtons();
    
}

void GUIJournal::initializeSortButtons()
{
    std::array<std::string, static_cast<size_t>(SortType::SORT_TYPE_COUNT)> sortButtonNames = {"All", "Current", "Completed"};
    sf::Vector2f basePosition = this->selectBox.getPosition();
    float offsetX = this->selectBox.getSize().x / static_cast<int>(SortType::SORT_TYPE_COUNT);
    float positionY = basePosition.y + this->selectBox.getSize().y / 2.f - journal::BUTTON_HEIGHT / 2.f;

    for(size_t i = 0; i < sortButtonNames.size(); ++i)
    {
        float positionX = basePosition.x - offsetX + offsetX * i;
        GUIButton sortButton = this->getJournalButtonStyle(
            sortButtonNames[i],
            sf::Vector2f(positionX, positionY),
            sf::Vector2f(this->selectBox.getSize().x / static_cast<int>(SortType::SORT_TYPE_COUNT), journal::BUTTON_HEIGHT),
            ButtonStyle::Bordered
        );
        SortType sortType = static_cast<SortType>(i);

        sortButton.setOnClick([this, sortType]()
        {
            this->sortType = sortType;
            this->sortAndDisplayQuests();

            for (auto& button : this->sortButtons)
                button.setDefaultState();

            this->sortButtons[static_cast<size_t>(sortType)].setSelectedState();
        });

        this->sortButtons.push_back(sortButton);
    }
}

GUIButton GUIJournal::getJournalButtonStyle(const std::string text, const sf::Vector2f position, const sf::Vector2f size, ButtonStyle style)
{
    return GUIButton(
        position,
        size,
        text,
        journal::SMALL_CHARACTER_SIZE * ConfigManager::getInstance().getScale(),
        journal::BUTTON_COLOR,
        journal::BUTTON_HOVER_COLOR,
        journal::BUTTON_ACTIVE_COLOR,
        style
    );
}

void GUIJournal::sortAndDisplayQuests()
{
    this->sortedQuests.clear();

    this->contentText.clear();

    for (auto& sortButton : this->sortButtons)
        sortButton.setDefaultState();

    switch (sortType)
    {
    case SortType::All:
        for (auto& quest : this->quests)
            if (quest.type != QuestType::Tutorial)
                this->sortedQuests.push_back(quest);
        this->sortButtons[0].setSelectedState();
        break;
    case SortType::Current:
        for (auto& quest : this->quests)
        {
            if (!quest.completed && quest.type != QuestType::Tutorial)
                this->sortedQuests.push_back(quest);
        }
        this->sortButtons[1].setSelectedState();
        break;
    case SortType::Completed:
        for (auto& quest : this->quests)
        {
            if (quest.completed && quest.type != QuestType::Tutorial)
                this->sortedQuests.push_back(quest);
        }
        this->sortButtons[2].setSelectedState();
        break;
    default:
        throw std::runtime_error("Invalid SortType for quests in journal");
    }

    this->scrollPosition = 0;
    this->setButtons();
}

void GUIJournal::setButtons()
{
    this->buttons.clear();

    for (auto& quest : this->sortedQuests)
    {
        GUIButton button = this->getJournalButtonStyle(
            quest.get().name,
            sf::Vector2f(
                this->selectBox.getPosition().x,
                this->selectBox.getPosition().y - this->selectBox.getSize().y / 2.f + (journal::BUTTON_HEIGHT / 2.f) + journal::DEFAULT_OUTLINE_THICKNESS + this->buttons.size() * journal::BUTTON_HEIGHT
            ),
            sf::Vector2f(this->selectBox.getSize().x, journal::BUTTON_HEIGHT)
        );

        button.setOnClick([this, &quest]()
        {
            this->setAllQuestsInactive();
            quest.get().active = true;
            this->setContentText();

            for (auto& button : this->buttons)
                button.setDefaultState();

            this->buttons[&quest - &this->sortedQuests[0]].setSelectedState();
        });

        this->buttons.push_back(button);
    }
}

void GUIJournal::setContentText()
{        
    this->contentText.clear();

    if (this->quests.empty()) return;        

    for (const auto& quest : this->quests | std::views::filter(&Quest::active))
    {
        sf::Text titleText = this->getJournalStyleText(quest.name);
        titleText.setPosition(
            this->contentBox.getPosition().x + this->contentBox.getSize().x / 2.f - titleText.getGlobalBounds().width / 2.f - journal::MARGIN,
            this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f + titleText.getGlobalBounds().height / 2.f + journal::MARGIN
        );
        this->contentText.push_back(titleText);

        for (int i = 0; i <= quest.currentStage; ++i)
        {
            sf::Color stageTextColor = (i == quest.currentStage && !quest.completed) ? journal::POSITIVE_TEXT_COLOR : sf::Color::White;
            sf::Text stageText = this->getJournalStyleText(quest.stages[i].description, journal::SMALL_CHARACTER_SIZE, stageTextColor);
            stageText.setPosition(
                this->contentBox.getPosition().x - this->contentBox.getSize().x / 2.f + stageText.getGlobalBounds().getSize().x / 2.f + journal::MARGIN,
                this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f + stageText.getGlobalBounds().getSize().y / 2.f + journal::MARGIN + i * journal::BUTTON_HEIGHT
            );
            this->contentText.push_back(stageText);

            if (i == quest.currentStage)
            {
                std::string statusString = quest.completed ? "Quest completed" : quest.stages[i].condition->getProgress();
                sf::Color statusColor = quest.completed ? journal::POSITIVE_TEXT_COLOR : sf::Color::White;
                sf::Text statusText = this->getJournalStyleText(statusString, journal::CHARACTER_SIZE, statusColor);
                statusText.setPosition(
                    this->contentBox.getPosition().x,
                    this->contentBox.getPosition().y + this->contentBox.getSize().y / 2.f - statusText.getGlobalBounds().getSize().y / 2.f - journal::MARGIN
                );
                this->contentText.push_back(statusText);
            }
        }
    }
}

void GUIJournal::setAllQuestsInactive()
{
    for (auto& quest : this->quests)
    {
        quest.active = false;
    }
}