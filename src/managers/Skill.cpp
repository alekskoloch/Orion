#include "Skill.h"

#include "../utils/Mouse.h"
#include "../utils/GraphicsOperations.h"

const float MARGIN = 120.f;

void Skill::initialize()
{
    TextureManager::getInstance().loadTexture(this->iconTextureName, ASSETS_PATH + this->iconTextureName + std::string(".png"));
    TextureManager::getInstance().loadTexture(this->iconHoverTextureName, ASSETS_PATH + this->iconHoverTextureName + std::string(".png"));
    TextureManager::getInstance().loadTexture(this->iconActiveTextureName , ASSETS_PATH + this->iconActiveTextureName + std::string(".png"));

    this->iconSprite = CreateSprite(this->iconTextureName);
    this->iconSprite.setPosition(this->iconPosition);

    this->initializeText();

    this->addCircleSegment();
}

void Skill::initializeText()
{
//TODO: get font from manager
    this->nameText.setString(this->name);
    this->nameText.setFont(this->font);
    this->nameText.setCharacterSize(30);
    this->nameText.setFillColor(sf::Color::White);
    this->nameText.setOutlineThickness(1);
    this->nameText.setOutlineColor(sf::Color::Black);

    this->descriptionText.setString(this->descriptions[0]);
    this->descriptionText.setFont(this->font);
    this->descriptionText.setCharacterSize(40);
    this->descriptionText.setFillColor(sf::Color::White);
    this->descriptionText.setOutlineThickness(1);
    this->descriptionText.setOutlineColor(sf::Color::Black);
}

void Skill::updateText()
{
    this->nameText.setOrigin(
        this->nameText.getGlobalBounds().width / 2.f,
        this->nameText.getGlobalBounds().height / 2.f
    );
    this->nameText.setPosition(
        this->iconSprite.getPosition().x,
        this->iconSprite.getPosition().y + MARGIN
    );

    this->descriptionText.setOrigin(
        this->descriptionText.getGlobalBounds().width / 2.f,
        this->descriptionText.getGlobalBounds().height / 2.f
    );
    this->descriptionText.setPosition(
        this->window.getSize().x / 2.f,
        this->window.getSize().y - MARGIN - this->descriptionText.getGlobalBounds().height
    );
}

void Skill::addCircleSegment()
{
    sf::Color segmentColor;
    if (this->requirements[this->currentLevel] == RequirementType::OrangeStone)
        segmentColor = sf::Color(195, 82, 20);
    else if (this->requirements[this->currentLevel] == RequirementType::GreenStone)
        segmentColor = sf::Color(0, 75, 73);
    else
        segmentColor = sf::Color::White;

    auto angle = 360.f / this->maxLevel;
    GUICircleSegment circleSegment(this->iconPosition, 100.f, this->currentLevel * angle, angle + (this->currentLevel * angle), 20.f, segmentColor);
    this->circleSegments.push_back(std::make_unique<GUICircleSegment>(circleSegment));
}   

void Skill::update()
{
    this->updateText();

    if (utils::isMouseOverSprite(this->iconSprite, sf::Mouse::getPosition(this->window)) && this->dialogBox.getState() == GUIDialogBoxState::Hidden)
    {
        if (this->currentLevel < this->maxLevel)
            this->circleSegments[this->currentLevel]->setState(CircleSegmentState::Hover);
        this->hover = true;
    }
    else
    {
        if (this->currentLevel < this->maxLevel)
            this->circleSegments[this->currentLevel]->setState(CircleSegmentState::Hidden);
        this->hover = false;
    }

    if (!this->active && this->dialogBox.getState() == GUIDialogBoxState::Hidden)
    {
        if (this->hover)
        {
            this->iconSprite.setTexture(TextureManager::getInstance().getTexture(this->iconHoverTextureName));
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (this->requirements[this->currentLevel] == RequirementType::None)
                    this->dialogBox.setMessage({"Are you sure you want to unlock", std::string(this->name + "?")});
                else if (this->requirements[this->currentLevel] == RequirementType::OrangeStone)
                    this->dialogBox.setMessage({std::string(this->name + " require Orange Stone") ,"Are you sure you want to unlock " + std::string(this->name + "?")});
                else if (this->requirements[this->currentLevel] == RequirementType::GreenStone)
                    this->dialogBox.setMessage({std::string(this->name + " require Green Stone") ,"Are you sure you want to unlock " + std::string(this->name + "?")});

                this->dialogBox.setType(GUIDialogBoxType::YesNo);
                this->dialogBox.setState(GUIDialogBoxState::Idle);
                this->dialogBox.setTarget(this->name);
            }
        }
        else
        {
            this->iconSprite.setTexture(TextureManager::getInstance().getTexture(this->iconTextureName));
        }
    }

    if (this->dialogBox.getState() != GUIDialogBoxState::Hidden && this->dialogBox.getTarget() == this->name)
    {
        if (this->dialogBox.getState() == GUIDialogBoxState::Yes)
        {
            this->dialogBox.setState(GUIDialogBoxState::Hidden);

            auto& playerStones = this->registry.get<StoneInventory>(this->registry.view<Player>().front());

            if (this->requirements[this->currentLevel] == RequirementType::OrangeStone)
            {
                if (playerStones.orangeStones > 0)
                    playerStones.orangeStones--;
                else
                {
                    this->dialogBox.setMessage({"You don't have the Orange Stone to unlock " + this->name});
                    this->dialogBox.setType(GUIDialogBoxType::Ok);
                    this->dialogBox.setState(GUIDialogBoxState::Idle);
                    return;
                }
            }
            else if (this->requirements[this->currentLevel] == RequirementType::GreenStone)
            {
                if (playerStones.greenStones > 0)
                    playerStones.greenStones--;
                else
                {
                    this->dialogBox.setMessage({"You don't have the Green Stone to unlock " + this->name});
                    this->dialogBox.setType(GUIDialogBoxType::Ok);
                    this->dialogBox.setState(GUIDialogBoxState::Idle);
                    return;
                }
            }

            this->onActivateFunctions[this->currentLevel](this->registry);

            //TODO: temporary solution for first skill
            if (this->name == "Orion protocol")
            {
                if (this->currentLevel == this->maxLevel - 1)
                    SkillManager::getInstance(this->window, this->registry).unlockSkills(this->skillsToUnlock);   
            }
            else if (this->currentLevel == 0)
            {
                SkillManager::getInstance(this->window, this->registry).unlockSkills(this->skillsToUnlock);
            }

            this->circleSegments.at(this->currentLevel)->setState(CircleSegmentState::Active);

            this->currentLevel++;

            if (this->currentLevel < this->maxLevel)
                this->addCircleSegment();

            if (this->currentLevel >= this->maxLevel)
            {
                this->active = true;
                this->iconSprite.setTexture(TextureManager::getInstance().getTexture(this->iconActiveTextureName));
            }
            else
            {
                this->descriptionText.setString(this->descriptions[this->currentLevel]);
            }
        }
        else if (this->dialogBox.getState() == GUIDialogBoxState::No)
        {
            this->dialogBox.setState(GUIDialogBoxState::Hidden);
        }
        else if (this->dialogBox.getState() == GUIDialogBoxState::Ok)
        {
            this->dialogBox.setState(GUIDialogBoxState::Hidden);
        }
    }

    //TODO: add dt
    for (auto& circleSegment : this->circleSegments)
    {
        circleSegment->update(0.016f);
    }
}

void Skill::draw()
{
    this->window.draw(this->iconSprite);

    this->window.draw(this->nameText);

    if (this->hover)
        this->window.draw(this->descriptionText);

    for (auto& circleSegment : this->circleSegments)
        this->window.draw(*circleSegment);
}