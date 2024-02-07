#pragma once

#include "GUISkill.h"
#include "SkillSystem.h"

class GUISkill;

class SkillBuilder
{
public:
    SkillBuilder& addWindow(sf::RenderWindow& window);
    SkillBuilder& addRegistry(entt::registry& registry);
    SkillBuilder& addDialogBox(GUIDialogBox& dialogBox);
    SkillBuilder& addIconPosition(sf::Vector2f iconPosition);
    SkillBuilder& addName(const std::string& name);
    SkillBuilder& addDescriptions(const std::vector<std::string>& descriptions);
    SkillBuilder& addIconTextureName(const std::string& iconTextureName);
    SkillBuilder& addOnActivateFunctions(const std::vector<std::pair<SkillType, float>>& onActivateFunctions);
    SkillBuilder& addRequirements(const std::vector<RequirementType>& requirements);
    SkillBuilder& addSkillsToUnlock(const std::vector<std::string>& skillsToUnlock);
    SkillBuilder& addMaxLevel(unsigned int maxLevel);
    SkillBuilder& addCurrentLevel(unsigned int currentLevel);
    SkillBuilder& addActiveStars(std::vector<std::unique_ptr<GUIStar>>& activeStars);

    std::unique_ptr<GUISkill> build();

private:
    sf::RenderWindow* window;
    entt::registry* registry;
    GUIDialogBox* dialogBox;
    sf::Vector2f iconPosition;
    std::string name;
    std::vector<std::string> descriptions;
    std::string iconTextureName;
    std::vector<std::pair<SkillType, float>> onActivateFunctions;
    std::vector<RequirementType> requirements;
    std::vector<std::string> skillsToUnlock;
    unsigned int maxLevel;
    unsigned int currentLevel;
    std::vector<std::unique_ptr<GUIStar>>* activeStars;
};