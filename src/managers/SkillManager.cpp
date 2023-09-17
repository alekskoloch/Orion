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
        skill.update();
}

void SkillManager::draw()
{
    for (auto& skill : this->skills)
        skill.draw();
}

void SkillManager::unlockSkills(std::vector<SkillSchema> skillsToUnlock)
{
    for (const SkillSchema& skillSchema : skillsToUnlock)
        this->addSkill(skillSchema);
}

void SkillManager::addSkill(const SkillSchema& skill)
{
    this->skills.push_back(Skill(
        this->window,
        this->registry,
        this->font,
        *this,
        skill.position,
        skill.name,
        skill.descriptions[0],
        skill.textureName,
        skill.hoverTextureName,
        skill.activeTextureName,
        skill.callbacks[0],
        skill.skillsToUnlock
    ));
}

void SkillManager::initializeFirstSkill()
{
    this->addSkill(OrionProtocol);
}