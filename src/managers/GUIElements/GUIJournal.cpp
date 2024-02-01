#include "GUIJournal.h"

GUIJournal::GUIJournal(sf::RenderWindow& window, entt::registry& registry, std::vector<Quest>& quests) : window(window), registry(registry), quests(quests)
{
    this->position = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
    this->size = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);

    sf::Vector2f contentBoxSize = sf::Vector2f(this->size.x * 0.8, this->size.y);
    sf::Vector2f selectBoxSize = sf::Vector2f(this->size.x * 0.2, this->size.y);
    sf::Vector2f bookmarkBarSize = sf::Vector2f(contentBoxSize.x + selectBoxSize.x, this->size.y * 0.1);

    //TODO: Refactor text initialization
    this->contentBox.setSize(contentBoxSize);
    this->contentBox.setFillColor(sf::Color(0, 0, 0, 200));
    this->contentBox.setOutlineColor(sf::Color::White);
    this->contentBox.setOutlineThickness(2.f);
    this->contentBox.setOrigin(this->contentBox.getSize().x / 2.f, this->contentBox.getSize().y / 2.f);
    this->contentBox.setPosition(position.x + selectBoxSize.x / 2.f, position.y + bookmarkBarSize.y / 2.f);

    this->selectBox.setSize(selectBoxSize);
    this->selectBox.setFillColor(sf::Color(0, 0, 0, 200));
    this->selectBox.setOutlineColor(sf::Color::White);
    this->selectBox.setOutlineThickness(2.f);
    this->selectBox.setOrigin(this->selectBox.getSize().x / 2.f, this->selectBox.getSize().y / 2.f);
    this->selectBox.setPosition(position.x - contentBoxSize.x / 2.f, position.y + bookmarkBarSize.y / 2.f);    

    this->bookmarkBar.setSize(bookmarkBarSize);
    this->bookmarkBar.setFillColor(sf::Color(0, 0, 0, 200));
    this->bookmarkBar.setOutlineColor(sf::Color::White);
    this->bookmarkBar.setOutlineThickness(2.f);
    this->bookmarkBar.setOrigin(this->bookmarkBar.getSize().x / 2.f, this->bookmarkBar.getSize().y / 2.f);
    this->bookmarkBar.setPosition(position.x, position.y - contentBoxSize.y / 2.f);

    this->font = FontManager::getInstance().getFont("font");
    this->titleText.setFont(font);
    this->titleText.setCharacterSize(40);
    this->titleText.setFillColor(sf::Color::White);
    this->titleText.setString("Journal");
    this->titleText.setOrigin(this->titleText.getGlobalBounds().width / 2.f, this->titleText.getGlobalBounds().height / 2.f);
    this->titleText.setPosition(position.x, position.y - contentBoxSize.y / 2.f);

    for (auto& quest : this->quests)
    {
        this->addButton(quest.name);
    }
}

void GUIJournal::processInput(sf::Event& event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) && this->isButtonReleased)
    {
        this->isOpen = !this->isOpen;
        this->isButtonReleased = false;
        this->selectedQuestIndex = -1;
        this->setAllButtonsInactive();
        this->contentText.clear();
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        this->isButtonReleased = true;

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        this->isMouseReleased = true;
}

void GUIJournal::update(sf::Time deltaTime)
{
    auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(this->window));
    this->updateButtons(mousePosition);
}

void GUIJournal::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->contentBox);
    target.draw(this->selectBox);
    target.draw(this->bookmarkBar);
    target.draw(this->titleText);

    for (auto& button : this->buttons)
    {
        target.draw(button.box);
        target.draw(button.text);
    }

    for (auto& text : this->contentText)
    {
        target.draw(text);
    }
}