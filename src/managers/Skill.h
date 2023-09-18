#pragma once

#include "../pch.h"

#include "SkillManager.h"
#include "GUIElements/GUIDialogBox.h"

#include "../schema/SkillSchema.h"
#include "../components/components.h"

class SkillManager;

class Skill
{
public:
    Skill(sf::RenderWindow& window, entt::registry& registry, sf::Font& font, sf::Vector2f iconPosition, std::string name, std::vector<std::string> descriptions, std::string iconTextureName, std::string iconHoverTextureName, std::string iconActiveTextureName, std::vector<std::function<void(entt::registry&)>> onActivateFunctions, std::vector<SkillSchema> skillToUnlock, unsigned int maxLevel, unsigned int currentLevel)
    : window(window), registry(registry), font(font), iconPosition(iconPosition), name(name), descriptions(descriptions), iconTextureName(iconTextureName), iconHoverTextureName(iconHoverTextureName), iconActiveTextureName(iconActiveTextureName), onActivateFunctions(onActivateFunctions), skillsToUnlock(skillToUnlock), dialogBox(window, "Are you sure you want to unlock this skill?", font), maxLevel(maxLevel), currentLevel(currentLevel)
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
    std::string iconHoverTextureName;
    std::string iconActiveTextureName;

    void initializeText();
    void updateText();

    std::string name;
    std::vector<std::string> descriptions;
    
    sf::Font& font;
    sf::Text nameText;
    sf::Text descriptionText;

    bool active = false;
    bool hover = false;

    std::vector<std::function<void(entt::registry&)>> onActivateFunctions;
    std::vector<SkillSchema> skillsToUnlock;

    GUIDialogBox dialogBox;
    bool dialogBoxActive = false;

    unsigned int maxLevel;
    unsigned int currentLevel;
};
