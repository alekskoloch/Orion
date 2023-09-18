#include "SkillManager.h"

SkillManager::SkillManager(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry)
{
    this->font.loadFromFile(ASSETS_PATH + std::string("fonts/font.ttf"));
    
    this->initializeFirstSkill();
}

void SkillManager::update()
{
    for (auto& skill : this->skills)
        skill->update();
}

void SkillManager::draw()
{
    for (auto& skill : this->skills)
        skill->draw();
}

void SkillManager::unlockSkills(std::vector<SkillSchema> skillsToUnlock)
{
    for (const SkillSchema& skillSchema : skillsToUnlock)
        this->addSkill(skillSchema);
}

void SkillManager::addSkill(SkillSchema skill)
{
    this->skills.push_back(std::make_unique<Skill>(
        this->window,
        this->registry,
        this->font,
        skill.position,
        skill.name,
        skill.descriptions,
        skill.textureName,
        skill.hoverTextureName,
        skill.activeTextureName,
        skill.callbacks,
        skill.skillsToUnlock,
        skill.maxLevel,
        skill.currentLevel
    ));  
}

void SkillManager::initializeFirstSkill()
{
    this->addSkill(OrionProtocol);
}