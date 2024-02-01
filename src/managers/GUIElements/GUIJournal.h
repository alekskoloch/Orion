#pragma once

#include "../../pch.h"

#include "../../managers/FontManager.h"

#include "../../systems/QuestCore/Quest.h"

class GUIJournal : public sf::Drawable
{

public:
    GUIJournal(sf::RenderWindow& window, entt::registry& registry, std::vector<Quest>& quests);
    
    bool isOpened() const { return this->isOpen; }

    void processInput(sf::Event& event);
    void update(sf::Time deltaTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    bool isOpen = false;
    bool isButtonReleased = false;
    bool isMouseReleased = false;

    std::vector<Quest>& quests;
    int selectedQuestIndex = -1;

    sf::Vector2f position;
    sf::Vector2f size;

    //TODO: Refactor this to a GUIBox class
    sf::RectangleShape contentBox;
    sf::RectangleShape bookmarkBar;
    sf::RectangleShape selectBox;
    sf::Font font;
    sf::Text titleText;

    std::vector<sf::Text> contentText;

    void setContentText()
    {
        float margin = 20.f;
        
        this->contentText.clear();

        if (!this->quests[this->selectedQuestIndex].completed)
        {
            for (int i = 0; i <= this->quests[this->selectedQuestIndex].currentStage; i++)
            {
                sf::Text text;
                text.setFont(this->font);
                text.setCharacterSize(20);
                if (i == this->quests[this->selectedQuestIndex].currentStage)
                    text.setFillColor(sf::Color::Green);
                else
                    text.setFillColor(sf::Color::White);
                text.setString(this->quests[this->selectedQuestIndex].stages[i].description);
                text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
                auto bounds = text.getGlobalBounds();
                text.setPosition(this->contentBox.getPosition().x - this->contentBox.getSize().x / 2.f + bounds.getSize().x / 2.f + margin, this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f + bounds.getSize().y / 2.f + margin + i * 50.f);
                this->contentText.push_back(text);

                if (i == this->quests[this->selectedQuestIndex].currentStage)
                {
                    sf::Text statusText;
                    statusText.setFont(this->font);
                    statusText.setCharacterSize(40);
                    statusText.setFillColor(sf::Color::White);
                    statusText.setString(this->quests[this->selectedQuestIndex].stages[i].condition->getProgress());
                    statusText.setOrigin(statusText.getGlobalBounds().width / 2.f, statusText.getGlobalBounds().height / 2.f);
                    auto statusBounds = statusText.getGlobalBounds();
                    statusText.setPosition(
                        this->contentBox.getPosition().x,
                        this->contentBox.getPosition().y + this->contentBox.getSize().y / 2.f - statusBounds.getSize().y / 2.f - margin
                    );
                    this->contentText.push_back(statusText);
                }
            }
        }
        else
        {
            for (int i = 0; i < this->quests[this->selectedQuestIndex].stages.size(); i++)
            {
                sf::Text text;
                text.setFont(this->font);
                text.setCharacterSize(20);
                text.setFillColor(sf::Color::White);
                text.setString(this->quests[this->selectedQuestIndex].stages[i].description);
                text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
                auto bounds = text.getGlobalBounds();
                text.setPosition(this->contentBox.getPosition().x - this->contentBox.getSize().x / 2.f + bounds.getSize().x / 2.f + margin, this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f + bounds.getSize().y / 2.f + margin + i * 50.f);
                this->contentText.push_back(text);
            }

            sf::Text statusText;
            statusText.setFont(this->font);
            statusText.setCharacterSize(40);
            statusText.setFillColor(sf::Color::Green);
            statusText.setString("Quest completed");
            statusText.setOrigin(statusText.getGlobalBounds().width / 2.f, statusText.getGlobalBounds().height / 2.f);
            auto bounds = statusText.getGlobalBounds();
            statusText.setPosition(
                        this->contentBox.getPosition().x,
                        this->contentBox.getPosition().y + this->contentBox.getSize().y / 2.f - bounds.getSize().y / 2.f - margin
                    );
            this->contentText.push_back(statusText);
        }
    }

    //TODO: Refactor this to a GUIButton class
    struct Button
    {
        sf::RectangleShape box;
        sf::Text text;

        sf::Color color = sf::Color::Transparent;
        sf::Color hoverColor = sf::Color(200, 0, 0, 200);
        sf::Color activeColor = sf::Color(0, 200, 0, 200);

        bool active = false;

        Button(sf::Vector2f position, sf::Vector2f size, sf::Font& font, std::string text)
        {
            this->box.setSize(size);
            this->box.setFillColor(sf::Color(0, 0, 0, 200));
            this->box.setOutlineColor(sf::Color::White);
            this->box.setOutlineThickness(2.f);
            this->box.setOrigin(this->box.getSize().x / 2.f, this->box.getSize().y / 2.f);
            this->box.setPosition(position);

            this->text.setFont(font);
            this->text.setCharacterSize(20);
            this->text.setFillColor(sf::Color::White);
            this->text.setString(text);
            this->text.setOrigin(this->text.getGlobalBounds().width / 2.f, this->text.getGlobalBounds().height / 2.f);
            this->text.setPosition(position);
        }
    };

    Button& activeButton()
    {
        return this->buttons[this->selectedQuestIndex];
    }

    void updateButtons(sf::Vector2f mousePosition)
    {
        for (auto& button : this->buttons)
        {
            if (button.box.getGlobalBounds().contains(mousePosition))
            {
                if (!button.active)
                    button.box.setFillColor(button.hoverColor);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->isMouseReleased)
                {
                    this->setAllButtonsInactive();
                    this->setAllQuestsInactive();
                    button.active = !button.active;
                    button.box.setFillColor(button.activeColor);
                    this->isMouseReleased = false;
                    this->selectedQuestIndex = &button - &this->buttons[0];
                    if (!this->quests[this->selectedQuestIndex].completed)
                        this->quests[this->selectedQuestIndex].active = true;
                    this->setContentText();
                }
            }
            else if (!button.active)
            {
                button.box.setFillColor(button.color);
            }
        }
    }

    void setAllButtonsInactive()
    {
        for (auto& button : this->buttons)
        {
            button.active = false;
            button.box.setFillColor(button.color);
        }
    }

    void setAllQuestsInactive()
    {
        for (auto& quest : this->quests)
        {
            quest.active = false;
        }
    }

    std::vector<Button> buttons;
    void addButtons(std::vector<std::string> buttonNames)
    {
        for (int i = 0; i < buttonNames.size(); i++)
        {
            sf::Vector2f position = this->selectBox.getPosition();
            position.y += i * 50.f;
            this->buttons.push_back(Button(position, sf::Vector2f(200.f, 40.f), this->font, buttonNames[i]));
        }
    }

    void addButton(std::string buttonName)
    {
        sf::Vector2f position = this->selectBox.getPosition();
        position.y += this->buttons.size() * 50.f;
        this->buttons.push_back(Button(position, sf::Vector2f(200.f, 40.f), this->font, buttonName));
    }
};