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
const unsigned int MIN_STARS = 10;
const unsigned int MAX_STARS = 30;
const float FLICKER_STAR_CHANCE = 0.5f;
const float COLOR_STAR_CHANCE = 0.4f;
const float MIN_BACKGROUND_STAR_RADIUS = 1.f;
const float MAX_BACKGROUND_STAR_RADIUS = 4.f;
const float MIN_IDLE_STAR_TIME = 0.2f;
const float MAX_IDLE_STAR_TIME = 0.7f;
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

//TODO: Split this class
class Skill
{
public:
    Skill(sf::RenderWindow& window, entt::registry& registry, GUIDialogBox& dialogBox, sf::Vector2f iconPosition, std::string name, std::vector<std::string> descriptions, std::string iconTextureName, std::vector<std::pair<SkillType, float>> onActivateFunctions, std::vector<RequirementType> requirements, std::vector<std::string> skillToUnlock, unsigned int maxLevel, unsigned int currentLevel, std::vector<std::unique_ptr<GUIStar>>& activeStars);
    void update(sf::Time& deltaTime);
    void draw();

private:
    sf::RenderWindow& window;
    entt::registry& registry;
    GUIDialogBox& dialogBox;

    sf::Vector2f iconPosition;
    sf::Sprite iconSprite;
    std::string iconTextureName;

    bool hover = false;
    bool hoverSoundPlayed = false;

    std::string name;
    std::vector<std::string> descriptions;
    sf::Text nameText;
    std::vector<sf::Text> descriptionTexts;

    std::vector<std::pair<SkillType, float>> onActivateFunctions;
    std::vector<RequirementType> requirements;
    std::vector<std::string> skillsToUnlock;

    bool isMaxLevelReached = false;
    unsigned int maxLevel;
    unsigned int currentLevel;

    bool isStarExists = false;
    std::vector<std::unique_ptr<GUIStar>>& activeStars;
    std::vector<std::unique_ptr<GUIStar>> stars;
    
    std::vector<std::unique_ptr<GUICircleSegment>> circleSegments;
    
    void initialize();
    void loadTexturesIntoManager();
    void initializeIconSprite();
    void initializeText();
    void initStarsForSkill();
    void addActiveStars();
    void addCircleSegment();
    void addDescriptionLine(const std::string descriptionTextLine);

    sf::Text getConfiguredText(std::string string, unsigned int characterSize, sf::Color textColor = DEFAULT_TEXT_COLOR);
    void centerText(sf::Text& text);

    sf::Color getStoneColor();
    sf::Vector2f calculateStarPosition();

    void updateHoverState();
    void handleHover();
    void handleBackgroundStars();
    void handleLeftClick();

    void updateDialogBox();
    std::vector<std::string> getMessagesForRequirements(RequirementType requirementType);
    void setErrorMessageInDialogBox();

    void handleSkillActivation();
    bool decrementStone(unsigned int& stoneCount);
    void applySkill();
    void updateDescription();
};
