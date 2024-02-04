#pragma once

#include "../../pch.h"

#include "../../managers/FontManager.h"

#include "../../systems/QuestCore/Quest.h"

#include "GUIButton.h"

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
        Completed
    };
    SortType sortType = SortType::All;
    void sortAndDisplayQuests();
    void setButtons();

    int scrollPosition = 0;
    int maxVisibleButtons;

    void setContentText()
    {
        float margin = 20.f;
        
        this->contentText.clear();

        if (this->quests.empty())
            return;        

        for (auto& quest : this->quests)
        {
            if (quest.active)
            {
                if (!quest.completed)
                {
                    sf::Text titleText;
                    titleText.setFont(this->font);
                    titleText.setCharacterSize(40);
                    titleText.setFillColor(sf::Color::White);
                    titleText.setString(quest.name);
                    titleText.setOrigin(titleText.getGlobalBounds().width / 2.f, titleText.getGlobalBounds().height / 2.f);
                    titleText.setPosition(this->contentBox.getPosition().x + this->contentBox.getSize().x / 2.f - titleText.getGlobalBounds().width / 2.f - margin, this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f + titleText.getGlobalBounds().height / 2.f + margin);
                    this->contentText.push_back(titleText);

                    for (int i = 0; i <= quest.currentStage; i++)
                    {
                        sf::Text text;
                        text.setFont(this->font);
                        text.setCharacterSize(20);
                        if (i == quest.currentStage)
                            text.setFillColor(sf::Color::Green);
                        else
                            text.setFillColor(sf::Color::White);
                        text.setString(quest.stages[i].description);
                        text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
                        auto bounds = text.getGlobalBounds();
                        text.setPosition(this->contentBox.getPosition().x - this->contentBox.getSize().x / 2.f + bounds.getSize().x / 2.f + margin, this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f + bounds.getSize().y / 2.f + margin + i * 50.f);
                        this->contentText.push_back(text);

                        if (i == quest.currentStage)
                        {
                            sf::Text statusText;
                            statusText.setFont(this->font);
                            statusText.setCharacterSize(40);
                            statusText.setFillColor(sf::Color::White);
                            statusText.setString(quest.stages[i].condition->getProgress());
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
                    sf::Text titleText;
                    titleText.setFont(this->font);
                    titleText.setCharacterSize(40);
                    titleText.setFillColor(sf::Color::White);
                    titleText.setString(quest.name);
                    titleText.setOrigin(titleText.getGlobalBounds().width / 2.f, titleText.getGlobalBounds().height / 2.f);
                    titleText.setPosition(this->contentBox.getPosition().x + this->contentBox.getSize().x / 2.f - titleText.getGlobalBounds().width / 2.f - margin, this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f + titleText.getGlobalBounds().height / 2.f + margin);
                    this->contentText.push_back(titleText);

                    for (int i = 0; i < quest.stages.size(); i++)
                    {
                        sf::Text text;
                        text.setFont(this->font);
                        text.setCharacterSize(20);
                        text.setFillColor(sf::Color::White);
                        text.setString(quest.stages[i].description);
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
        }

        // if (!this->quests[this->selectedQuestIndex].completed)
        // {
        //     for (int i = 0; i <= this->quests[this->selectedQuestIndex].currentStage; i++)
        //     {
        //         sf::Text text;
        //         text.setFont(this->font);
        //         text.setCharacterSize(20);
        //         if (i == this->quests[this->selectedQuestIndex].currentStage)
        //             text.setFillColor(sf::Color::Green);
        //         else
        //             text.setFillColor(sf::Color::White);
        //         text.setString(this->quests[this->selectedQuestIndex].stages[i].description);
        //         text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
        //         auto bounds = text.getGlobalBounds();
        //         text.setPosition(this->contentBox.getPosition().x - this->contentBox.getSize().x / 2.f + bounds.getSize().x / 2.f + margin, this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f + bounds.getSize().y / 2.f + margin + i * 50.f);
        //         this->contentText.push_back(text);

        //         if (i == this->quests[this->selectedQuestIndex].currentStage)
        //         {
        //             sf::Text statusText;
        //             statusText.setFont(this->font);
        //             statusText.setCharacterSize(40);
        //             statusText.setFillColor(sf::Color::White);
        //             statusText.setString(this->quests[this->selectedQuestIndex].stages[i].condition->getProgress());
        //             statusText.setOrigin(statusText.getGlobalBounds().width / 2.f, statusText.getGlobalBounds().height / 2.f);
        //             auto statusBounds = statusText.getGlobalBounds();
        //             statusText.setPosition(
        //                 this->contentBox.getPosition().x,
        //                 this->contentBox.getPosition().y + this->contentBox.getSize().y / 2.f - statusBounds.getSize().y / 2.f - margin
        //             );
        //             this->contentText.push_back(statusText);
        //         }
        //     }
        // }
        // else
        // {
        //     for (int i = 0; i < this->quests[this->selectedQuestIndex].stages.size(); i++)
        //     {
        //         sf::Text text;
        //         text.setFont(this->font);
        //         text.setCharacterSize(20);
        //         text.setFillColor(sf::Color::White);
        //         text.setString(this->quests[this->selectedQuestIndex].stages[i].description);
        //         text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
        //         auto bounds = text.getGlobalBounds();
        //         text.setPosition(this->contentBox.getPosition().x - this->contentBox.getSize().x / 2.f + bounds.getSize().x / 2.f + margin, this->contentBox.getPosition().y - this->contentBox.getSize().y / 2.f + bounds.getSize().y / 2.f + margin + i * 50.f);
        //         this->contentText.push_back(text);
        //     }

        //     sf::Text statusText;
        //     statusText.setFont(this->font);
        //     statusText.setCharacterSize(40);
        //     statusText.setFillColor(sf::Color::Green);
        //     statusText.setString("Quest completed");
        //     statusText.setOrigin(statusText.getGlobalBounds().width / 2.f, statusText.getGlobalBounds().height / 2.f);
        //     auto bounds = statusText.getGlobalBounds();
        //     statusText.setPosition(
        //                 this->contentBox.getPosition().x,
        //                 this->contentBox.getPosition().y + this->contentBox.getSize().y / 2.f - bounds.getSize().y / 2.f - margin
        //             );
        //     this->contentText.push_back(statusText);
        // }
    }

    void setAllQuestsInactive()
    {
        for (auto& quest : this->quests)
        {
            quest.active = false;
        }
    }
};