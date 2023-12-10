#include "SkillBuilder.h"

SkillBuilder& SkillBuilder::addWindow(sf::RenderWindow& window)
{
    this->window = &window;
    return *this;
}

SkillBuilder& SkillBuilder::addRegistry(entt::registry& registry)
{
    this->registry = &registry;
    return *this;
}

SkillBuilder& SkillBuilder::addDialogBox(GUIDialogBox& dialogBox)
{
    this->dialogBox = &dialogBox;
    return *this;
}

SkillBuilder& SkillBuilder::addIconPosition(sf::Vector2f iconPosition)
{
    this->iconPosition = iconPosition;
    return *this;
}

SkillBuilder& SkillBuilder::addName(const std::string& name)
{
    this->name = name;
    return *this;
}

SkillBuilder& SkillBuilder::addDescriptions(const std::vector<std::string>& descriptions)
{
    this->descriptions = descriptions;
    return *this;
}

SkillBuilder& SkillBuilder::addIconTextureName(const std::string& iconTextureName)
{
    this->iconTextureName = iconTextureName;
    return *this;
}

SkillBuilder& SkillBuilder::addOnActivateFunctions(const std::vector<std::pair<SkillType, float>>& onActivateFunctions)
{
    this->onActivateFunctions = onActivateFunctions;
    return *this;
}

SkillBuilder& SkillBuilder::addRequirements(const std::vector<RequirementType>& requirements)
{
    this->requirements = requirements;
    return *this;
}

SkillBuilder& SkillBuilder::addSkillsToUnlock(const std::vector<std::string>& skillsToUnlock)
{
    this->skillsToUnlock = skillsToUnlock;
    return *this;
}

SkillBuilder& SkillBuilder::addMaxLevel(unsigned int maxLevel)
{
    this->maxLevel = maxLevel;
    return *this;
}

SkillBuilder& SkillBuilder::addCurrentLevel(unsigned int currentLevel)
{
    this->currentLevel = currentLevel;
    return *this;
}

SkillBuilder& SkillBuilder::addActiveStars(std::vector<std::unique_ptr<GUIStar>>& activeStars)
{
    this->activeStars = &activeStars;
    return *this;
}

std::unique_ptr<Skill> SkillBuilder::build()
{
    return std::make_unique<Skill>(*window, *registry, *dialogBox, iconPosition, name, descriptions, iconTextureName, onActivateFunctions, requirements, skillsToUnlock, maxLevel, currentLevel, *activeStars);
}