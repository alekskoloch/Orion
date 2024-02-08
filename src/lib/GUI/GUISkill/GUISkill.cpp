#include "pch.h"
#include "GUISkill.h"

GUISkill::GUISkill(sf::RenderWindow& window, entt::registry& registry, GUIDialogBox& dialogBox, sf::Vector2f iconPosition, std::string name, std::vector<std::string> descriptions, std::string iconTextureName, std::vector<std::pair<SkillType, float>> onActivateFunctions, std::vector<RequirementType> requirements, std::vector<std::string> skillToUnlock, unsigned int maxLevel, unsigned int currentLevel, std::vector<std::unique_ptr<GUIStar>>& activeStars)
    :   window(window),
        registry(registry),
        dialogBox(dialogBox),
        iconPosition(iconPosition),
        name(name),
        descriptions(descriptions),
        iconTextureName(iconTextureName),
        onActivateFunctions(onActivateFunctions),
        requirements(requirements),
        skillsToUnlock(skillToUnlock),
        maxLevel(maxLevel),
        currentLevel(currentLevel),
        activeStars(activeStars)
    {
        this->initialize();
    }

void GUISkill::initialize()
{
    this->loadTexturesIntoManager();
    this->initializeIconSprite();
    this->initializeText();
    this->addCircleSegment();
}

void GUISkill::loadTexturesIntoManager()
{
    TextureManager::getInstance().loadTexture(this->iconTextureName, ASSETS_PATH + std::string("skillAssets/") + this->iconTextureName + ".png");
    TextureManager::getInstance().loadTexture(this->iconTextureName + "Hover", ASSETS_PATH + std::string("skillAssets/") + this->iconTextureName + "Hover.png");
    TextureManager::getInstance().loadTexture(this->iconTextureName + "Active", ASSETS_PATH + std::string("skillAssets/") + this->iconTextureName + "Active.png");
}

void GUISkill::initializeIconSprite()
{
    this->iconSprite = CreateSprite(this->iconTextureName);
    this->iconSprite.setPosition(this->iconPosition);
}

void GUISkill::initializeText()
{
    this->nameText = this->getConfiguredText(this->name, NAME_CHARACTER_SIZE);
    this->centerText(this->nameText);
    this->nameText.setPosition(this->iconSprite.getPosition().x, this->iconSprite.getPosition().y + MARGIN);

    this->addDescriptionLine(this->descriptions[0]);
}

void GUISkill::addCircleSegment()
{
    auto angle = 360.f / this->maxLevel;
    this->circleSegments.push_back(std::make_unique<GUICircleSegment>(
        this->iconPosition,
        90.f,
        this->currentLevel * angle,
        angle + (this->currentLevel * angle),
        SEGMENT_THICKNESS,
        this->getStoneColor()
    ));
}   

void GUISkill::addActiveStars()
{
    for (auto& star : this->stars)
    {
        auto newStar = star->getStar();
        this->activeStars.push_back(std::make_unique<GUIStar>(newStar));
    }

    this->stars.clear();
}

void GUISkill::initStarsForSkill()
{
    unsigned int numStars = ProceduralGenerationSystem::GetRandomNumber(MIN_STARS, MAX_STARS);

    for (int i = 0; i < numStars; i++)
    {
        bool flicker = (ProceduralGenerationSystem::GetRandomNumber(0.f, 1.f) <= FLICKER_STAR_CHANCE);
        sf::Color color = sf::Color::White;

        if (i == 0 || ProceduralGenerationSystem::GetRandomNumber(0.f, 1.f) <= COLOR_STAR_CHANCE)
        {
            color = this->getStoneColor();
        }

        GUIStar star(
            this->calculateStarPosition(),
            color,
            ProceduralGenerationSystem::GetRandomNumber(MIN_BACKGROUND_STAR_RADIUS, MAX_BACKGROUND_STAR_RADIUS),
            flicker,
            ProceduralGenerationSystem::GetRandomNumber(MIN_IDLE_STAR_TIME, MAX_IDLE_STAR_TIME)
        );

        this->stars.push_back(std::make_unique<GUIStar>(star));
    }

    this->isStarExists = true;
}

void GUISkill::update(sf::Time& deltaTime)
{
    if (this->dialogBox.getState() == GUIDialogBoxState::Hidden)
    {
        this->updateHoverState();

        if (this->hover)
        {
            this->handleHover();
            this->handleBackgroundStars();
            this->handleLeftClick();
        }
        else if (!this->isMaxLevelReached)
        {
            this->circleSegments[this->currentLevel]->setState(CircleSegmentState::Hidden);

            if (this->isStarExists)
            {
                this->isStarExists = false;
                this->stars.clear();
            }

            this->iconSprite.setTexture(TextureManager::getInstance().getTexture(this->iconTextureName));
        }
    }

    this->updateDialogBox();

    for (auto& circleSegment : this->circleSegments)
    {
        circleSegment->update(deltaTime.asSeconds());
    }

    for (auto& star : this->stars)
    {
        star->update(deltaTime.asSeconds());
    }
}

void GUISkill::draw()
{
    for (auto& star : this->stars)
        this->window.draw(*star);

    this->window.draw(this->iconSprite);

    this->window.draw(this->nameText);

    sf::View view = window.getView();

    this->window.setView(this->window.getDefaultView());

    if (this->hover)
        for (auto& descriptionText : this->descriptionTexts)
            this->window.draw(descriptionText);

    this->window.setView(view);

    for (auto& circleSegment : this->circleSegments)
        this->window.draw(*circleSegment);
}

sf::Text GUISkill::getConfiguredText(std::string string, unsigned int characterSize, sf::Color textColor)
{
    sf::Text text;
    text.setFont(FontManager::getInstance().getFont("font"));
    text.setString(string);
    text.setCharacterSize(characterSize);
    text.setFillColor(textColor);
    text.setOutlineThickness(1);
    text.setOutlineColor(sf::Color::Black);
    return text;
}

void GUISkill::centerText(sf::Text& text)
{
    text.setOrigin(
        text.getGlobalBounds().width / 2.f,
        text.getGlobalBounds().height / 2.f
    );
    text.setPosition(
        this->window.getSize().x / 2.f,
        this->window.getSize().y / 2.f
    );
}

sf::Color GUISkill::getStoneColor()
{
    if (this->requirements[this->currentLevel] == RequirementType::OrangeStone)
        return ORANGE_STONE_COLOR;
    else if (this->requirements[this->currentLevel] == RequirementType::GreenStone)
        return GREEN_STONE_COLOR;
    else if (this->requirements[this->currentLevel] == RequirementType::YellowStone)
        return YELLOW_STONE_COLOR;
    else
        return DEFAULT_STONE_COLOR;
}

//TODO: Load values from config 
sf::Vector2f GUISkill::calculateStarPosition()
{
    float offset = !ProceduralGenerationSystem::GetRandomNumber(0,2) ? ProceduralGenerationSystem::GetRandomNumber(1.1f, 2.f) : 1.f;

    return sf::Vector2f(
                this->iconPosition.x + (ProceduralGenerationSystem::GetRandomNumber(-300.f, 300.f) * offset),
                this->iconPosition.y + (ProceduralGenerationSystem::GetRandomNumber(-300.f, 300.f) * offset)
            );
}

void GUISkill::addDescriptionLine(const std::string descriptionTextLine)
{
    this->descriptionTexts.push_back(this->getConfiguredText(descriptionTextLine, DESCRIPTION_CHARACTER_SIZE, INACTIVE_DESCRIPTION_COLOR));
    this->centerText(this->descriptionTexts[this->descriptionTexts.size() - 1]);

    for (int i = 0; i < this->descriptionTexts.size(); i++)
    {
        this->descriptionTexts[this->descriptionTexts.size() - 1 - i].setPosition(
            this->window.getSize().x / 2.f,
            this->window.getSize().y - MARGIN - (this->descriptionTexts[this->descriptionTexts.size() - 1 - i].getGlobalBounds().height * (i + 1))
        );
    }
}

void GUISkill::updateHoverState()
{
    if (utils::isMouseOverSprite(this->iconSprite, utils::getMousePositionInWindow(this->window)) && this->dialogBox.getState() == GUIDialogBoxState::Hidden)
        this->hover = true;
    else
    {
        this->hoverSoundPlayed = false;
        this->hover = false;
    }
}

void GUISkill::handleHover()
{
    if (!this->hoverSoundPlayed)
    {
        this->hoverSoundPlayed = true;
        SoundManager::getInstance().playSound("SkillHover");
    }

    if (this->currentLevel < this->maxLevel)
    {
        this->circleSegments[this->currentLevel]->setState(CircleSegmentState::Hover);
        this->iconSprite.setTexture(TextureManager::getInstance().getTexture(std::string(this->iconTextureName + "Hover")));
    
        if (!this->isStarExists)
            this->initStarsForSkill();
    }
}

void GUISkill::handleBackgroundStars()
{
    
}

void GUISkill::handleLeftClick()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->isMaxLevelReached)
    {
        if (ExperienceSystem::getSkillPoints(this->registry) > 0)
        {
            this->dialogBox.setMessage(this->getMessagesForRequirements(this->requirements[this->currentLevel]));

            this->dialogBox.setType(GUIDialogBoxType::YesNo);
            this->dialogBox.setState(GUIDialogBoxState::Idle);
            this->dialogBox.setTarget(this->name);
        }
        else
        {
            this->setErrorMessageInDialogBox();
        }
    }
}

void GUISkill::updateDialogBox()
{
    if (this->dialogBox.getState() != GUIDialogBoxState::Hidden && this->dialogBox.getTarget() == this->name)
    {
        if (this->dialogBox.getState() == GUIDialogBoxState::Yes)
        {
            this->dialogBox.setState(GUIDialogBoxState::Hidden);
            this->handleSkillActivation();
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
}

std::vector<std::string> GUISkill::getMessagesForRequirements(RequirementType requirementType)
{
    std::vector<std::string> messages;
    messages.push_back("Are you sure you want to unlock " + std::string(this->name + "?"));

    switch (requirementType)
    {
    case RequirementType::None:
        break;
    case RequirementType::OrangeStone:
        messages.insert(messages.begin(), std::string(this->name + " require Orange Stone"));
        break;
    case RequirementType::GreenStone:
        messages.insert(messages.begin(), std::string(this->name + " require Green Stone"));
        break;
    case RequirementType::YellowStone:
        messages.insert(messages.begin(), std::string(this->name + " require Orange and Green Stone"));
        break;
    case RequirementType::RedStone:
        throw std::invalid_argument("Red Stone is not implemented");
        break;
    case RequirementType::BlueStone:
        throw std::invalid_argument("Blue Stone is not implemented");
        break;
    case RequirementType::PowerfulOrangeStone:
        throw std::invalid_argument("Powerful Orange Stone is not implemented");
        break;
    case RequirementType::PowerfulGreenStone:
        throw std::invalid_argument("Powerful Green Stone is not implemented");
        break;
    case RequirementType::PowerfulYelowStone:
        throw std::invalid_argument("Powerful Yellow Stone is not implemented");
        break;
    case RequirementType::PowerfulRedStone:
        throw std::invalid_argument("Powerful Red Stone is not implemented");
        break;
    case RequirementType::PowerfulBlueStone:
        throw std::invalid_argument("Powerful Blue Stone is not implemented");
        break;
    default:
        throw std::invalid_argument("Invalid requirement type");
        break;
    }

    return messages;
}

void GUISkill::setErrorMessageInDialogBox()
{
    SoundManager::getInstance().playSound("Error");

    if (ExperienceSystem::getSkillPoints(this->registry) <= 0)
        this->dialogBox.setMessage({"You don't have any Skill Points"});
    else
        switch (this->requirements[this->currentLevel])
        {
        case RequirementType::OrangeStone:
            this->dialogBox.setMessage({"You don't have the Orange Stone to unlock " + this->name});
            break;
        case RequirementType::GreenStone:
            this->dialogBox.setMessage({"You don't have the Green Stone to unlock " + this->name});
            break;
        case RequirementType::YellowStone:
            this->dialogBox.setMessage({"You don't have the Yellow Stone to unlock " + this->name});
            break;
        case RequirementType::RedStone:
            throw std::invalid_argument("Red Stone is not implemented");
            break;
        case RequirementType::BlueStone:
            throw std::invalid_argument("Blue Stone is not implemented");
            break;
        case RequirementType::PowerfulOrangeStone:
            throw std::invalid_argument("Powerful Orange Stone is not implemented");
            break;
        case RequirementType::PowerfulGreenStone:
            throw std::invalid_argument("Powerful Green Stone is not implemented");
            break;
        case RequirementType::PowerfulYelowStone:
            throw std::invalid_argument("Powerful Yellow Stone is not implemented");
            break;
        case RequirementType::PowerfulRedStone:
            throw std::invalid_argument("Powerful Red Stone is not implemented");
            break;
        case RequirementType::PowerfulBlueStone:
            throw std::invalid_argument("Powerful Blue Stone is not implemented");
            break;
        default:
            throw std::invalid_argument("Invalid requirement type");
            break;
        }

    this->dialogBox.setType(GUIDialogBoxType::Ok);
    this->dialogBox.setState(GUIDialogBoxState::Idle);
    this->dialogBox.setTarget(this->name);
}

void GUISkill::handleSkillActivation()
{
    auto& playerStones = this->registry.get<StoneInventory>(this->registry.view<Player>().front());

    switch (this->requirements[this->currentLevel])
    {
    case RequirementType::None:
        break;
    case RequirementType::OrangeStone:
        if (!this->decrementStone(playerStones.orangeStones)) return;
        break;
    case RequirementType::GreenStone:
        if (!this->decrementStone(playerStones.greenStones)) return;
        break;
    case RequirementType::YellowStone:
        throw std::invalid_argument("Yellow Stone is not implemented");
        break;
    case RequirementType::RedStone:
        throw std::invalid_argument("Red Stone is not implemented");
        break;
    case RequirementType::BlueStone:
        throw std::invalid_argument("Blue Stone is not implemented");
        break;
    case RequirementType::PowerfulOrangeStone:
        throw std::invalid_argument("Powerful Orange Stone is not implemented");
        break;
    case RequirementType::PowerfulGreenStone:
        throw std::invalid_argument("Powerful Green Stone is not implemented");
        break;
    case RequirementType::PowerfulYelowStone:
        throw std::invalid_argument("Powerful Yellow Stone is not implemented");
        break;
    case RequirementType::PowerfulRedStone:
        throw std::invalid_argument("Powerful Red Stone is not implemented");
        break;
    case RequirementType::PowerfulBlueStone:
        throw std::invalid_argument("Powerful Blue Stone is not implemented");
        break;
    default:
        break;
    }

    SoundManager::getInstance().playSound("SkillUnlock");
    ExperienceSystem::removeSkillPoint(this->registry);
    this->applySkill();
    this->circleSegments.at(this->currentLevel)->setState(CircleSegmentState::Active);
    this->addActiveStars();
    this->currentLevel++;

    if (this->currentLevel < this->maxLevel)
    {
        this->addCircleSegment();
    }
    else if (this->currentLevel == this->maxLevel)
    {
        this->isMaxLevelReached = true;
        this->iconSprite.setTexture(TextureManager::getInstance().getTexture(std::string(this->iconTextureName + "Active")));
    }

    this->updateDescription();
}

bool GUISkill::decrementStone(unsigned int& stoneCount)
{
    if (stoneCount > 0)
    {
        stoneCount--;
        return true;
    }
    else
    {
        this->setErrorMessageInDialogBox();
        return false;
    }
}

void GUISkill::applySkill()
{
    if (this->currentLevel == 0)
        SkillManager::getInstance(this->window, this->registry).unlockSkills(this->skillsToUnlock);

    SkillSystem::modifySkill(this->registry, this->onActivateFunctions[this->currentLevel].first, this->onActivateFunctions[this->currentLevel].second);
}

void GUISkill::updateDescription()
{
    this->descriptionTexts[this->descriptionTexts.size() - 1].setFillColor(ACTIVE_DESCRIPTION_COLOR);
    if (this->currentLevel < this->maxLevel)
        this->addDescriptionLine(this->descriptions[this->currentLevel]);
}