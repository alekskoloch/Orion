#include "SkillManager.h"

SkillManager::SkillManager(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry), dialogBox(window, "Are you sure you want to unlock this skill?", this->font)
{
    this->font.loadFromFile(ASSETS_PATH + std::string("fonts/font.ttf"));

    this->dialogBox.setState(GUIDialogBoxState::Hidden);
    
    this->initializeFirstSkill();
}

void SkillManager::update()
{
    for (auto& skill : this->skills)
        skill->update();

    if (this->dialogBox.getState() != GUIDialogBoxState::Hidden)
        this->dialogBox.update();
}

void SkillManager::draw()
{
    for (auto& skill : this->skills)
        skill->draw();

    if (this->dialogBox.getState() != GUIDialogBoxState::Hidden)
        this->dialogBox.draw();
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
        this->dialogBox,
        skill.position,
        skill.name,
        skill.descriptions,
        skill.textureName,
        skill.hoverTextureName,
        skill.activeTextureName,
        skill.callbacks,
        skill.requirements,
        skill.skillsToUnlock,
        skill.maxLevel,
        skill.currentLevel
    ));  
}

void SkillManager::initializeFirstSkill()
{
    this->addSkill(OrionProtocol);
}