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
        GUIButton button;
        button = GUIButton(
            sf::Vector2f(
                this->selectBox.getPosition().x,
                this->selectBox.getPosition().y - this->selectBox.getSize().y / 2.f + 25.f + 2.f + this->buttons.size() * 50.f
            ),
            sf::Vector2f(this->selectBox.getSize().x, 50.f),
            quest.name,
            20,
            sf::Color(0, 0, 0, 200),
            sf::Color(0, 100, 0, 200),
            sf::Color(0, 200, 0, 200),
            ButtonStyle::Borderless
        );

        button.setOnClick([this, &quest]()
        {
            for (auto& q : this->quests)
            {
                q.active = false;
            }

            quest.active = true;

            this->setContentText();
        });

        this->buttons.push_back(button);
    }

    this->maxVisibleButtons = std::floor(this->selectBox.getSize().y / 50.f);
}

void GUIJournal::processInput(sf::Event& event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) && this->isButtonReleased)
    {
        this->isOpen = !this->isOpen;
        this->isButtonReleased = false;
        this->contentText.clear();
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        this->isButtonReleased = true;

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        this->isMouseReleased = true;

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

void GUIJournal::update(sf::Time deltaTime)
{
    auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

    if (this->buttons.size() > this->maxVisibleButtons)
    {
        for (int i = this->scrollPosition, j = 0; i < this->maxVisibleButtons + this->scrollPosition; i++, j++)
        {
            this->buttons[i].setPosition(
                sf::Vector2f(
                    this->selectBox.getPosition().x,
                    this->selectBox.getPosition().y - this->selectBox.getSize().y / 2.f + 25.f + 2.f + j * 50.f
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
}