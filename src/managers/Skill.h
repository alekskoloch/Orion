#pragma once

#include "../pch.h"

#include "../systems/SkillSystem.h"

#include "SkillManager.h"
#include "GUIElements/GUIDialogBox.h"
#include "GUIElements/GUICircleSegment.h"
#include "GUIElements/GUIStar.h"

#include "../components/components.h"

//TODO: Move to config
const float MARGIN = 120.f;
const float SEGMENT_THICKNESS = 5.f;
const unsigned int NAME_CHARACTER_SIZE = 30;
const unsigned int DESCRIPTION_CHARACTER_SIZE = 40;
const sf::Color ORANGE_STONE_COLOR = sf::Color(195, 82, 20);
const sf::Color GREEN_STONE_COLOR = sf::Color(0, 75, 73);
const sf::Color YELLOW_STONE_COLOR = sf::Color(195, 82, 150, 100);
const sf::Color DEFAULT_STONE_COLOR = sf::Color::White;
const sf::Color DEFAULT_TEXT_COLOR = sf::Color::White;
const sf::Color ACTIVE_DESCRIPTION_COLOR = sf::Color::White;
const sf::Color INACTIVE_DESCRIPTION_COLOR = sf::Color(150, 150, 150);

class SkillManager;

class Skill
{
public:
    Skill(sf::RenderWindow& window, entt::registry& registry, GUIDialogBox& dialogBox, sf::Vector2f iconPosition, std::string name, bool multiLevel, std::vector<std::string> descriptions, std::string iconTextureName, std::vector<std::pair<SkillType, float>> onActivateFunctions, std::vector<RequirementType> requirements, std::vector<std::string> skillToUnlock, unsigned int maxLevel, unsigned int currentLevel, std::vector<std::unique_ptr<GUIStar>>& activeStars)
    : window(window), registry(registry), dialogBox(dialogBox), iconPosition(iconPosition), name(name), multiLevel(multiLevel), descriptions(descriptions), iconTextureName(iconTextureName), onActivateFunctions(onActivateFunctions), requirements(requirements), skillsToUnlock(skillToUnlock), maxLevel(maxLevel), currentLevel(currentLevel), activeStars(activeStars)
    {
        this->initialize();
    }

    void update();
    void draw();

private:
    sf::RenderWindow& window;
    entt::registry& registry;

    void initialize();

    sf::Sprite iconSprite;
    sf::Vector2f iconPosition;

    std::string iconTextureName;

    void initializeText();

    std::string name;
    std::vector<std::string> descriptions;
    bool multiLevel;

    sf::Text nameText;
    std::vector<sf::Text> descriptionTexts;

    bool isMaxLevelReached = false;
    bool hover = false;

    std::vector<std::pair<SkillType, float>> onActivateFunctions;
    std::vector<RequirementType> requirements;
    std::vector<std::string> skillsToUnlock;

    GUIDialogBox& dialogBox;

    unsigned int maxLevel;
    unsigned int currentLevel;

    std::vector<std::unique_ptr<GUICircleSegment>> circleSegments;
    void addCircleSegment();

    std::vector<std::unique_ptr<GUIStar>>& activeStars;
    std::vector<std::unique_ptr<GUIStar>> stars;
    void addActiveStars();
    void initStarsForSkill();
    bool isStarExists = false;

    void loadTexturesIntoManager();
    void initializeIconSprite();
    sf::Text getConfiguredText(std::string string, unsigned int characterSize, sf::Color textColor = DEFAULT_TEXT_COLOR);
    void centerText(sf::Text& text);

    sf::Color getStoneColor();
    sf::Vector2f calculateStarPosition();
    void addDescriptionLine(const std::string descriptionTextLine);
    void updateHoverState();
    std::vector<std::string> getMessagesForRequirements(RequirementType requirementType);
    void setErrorMessageInDialogBox();

    void handleHover();
    void handleBackgroundStars();
    void handleLeftClick();

    void updateDialogBox();
    void handleSkillActivation();
    bool decrementStone(unsigned int& stoneCount);

    void applySkill();
};
