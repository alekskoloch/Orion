#pragma once

#include "../pch.h"

#include "SkillManager.h"
#include "GUIElements/GUIDialogBox.h"
#include "GUIElements/GUICircleSegment.h"
#include "GUIElements/GUIStar.h"

#include "../schema/SkillSchema.h"
#include "../components/components.h"

class SkillManager;

class Skill
{
public:
    Skill(sf::RenderWindow& window, entt::registry& registry, sf::Font& font, GUIDialogBox& dialogBox, sf::Vector2f iconPosition, std::string name, bool multiLevel, std::vector<std::string> descriptions, std::string iconTextureName, std::vector<std::pair<SkillType, float>> onActivateFunctions, std::vector<RequirementType> requirements, std::vector<std::string> skillToUnlock, unsigned int maxLevel, unsigned int currentLevel, std::vector<std::unique_ptr<GUIStar>>& activeStars)
    : window(window), registry(registry), font(font), dialogBox(dialogBox), iconPosition(iconPosition), name(name), multiLevel(multiLevel), descriptions(descriptions), iconTextureName(iconTextureName), onActivateFunctions(onActivateFunctions), requirements(requirements), skillsToUnlock(skillToUnlock), maxLevel(maxLevel), currentLevel(currentLevel), activeStars(activeStars)
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
    void updateText();

    std::string name;
    std::vector<std::string> descriptions;
    bool multiLevel;
    
    sf::Font& font;
    sf::Text nameText;
    std::vector<sf::Text> descriptionTexts;

    bool active = false;
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
};
