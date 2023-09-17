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
    Skill(sf::RenderWindow& window, entt::registry& registry, sf::Font& font, sf::Vector2f iconPosition, std::string name, std::string description, std::string iconTextureName, std::string iconHoverTextureName, std::string iconActiveTextureName, std::function<void(entt::registry&)> onActivate, std::vector<SkillSchema> skillToUnlock)
    : window(window), registry(registry), font(font), iconPosition(iconPosition), name(name), description(description), iconTextureName(iconTextureName), iconHoverTextureName(iconHoverTextureName), iconActiveTextureName(iconActiveTextureName), onActivate(onActivate), skillsToUnlock(skillToUnlock), dialogBox(window, "Are you sure you want to unlock this skill?", font)
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
    std::string description;
    
    sf::Font& font;
    sf::Text nameText;
    sf::Text descriptionText;

    bool active = false;
    bool hover = false;

    std::function<void(entt::registry&)> onActivate;
    std::vector<SkillSchema> skillsToUnlock;

    GUIDialogBox dialogBox;
    bool dialogBoxActive = false;
};
