#include "SkillManager.h"

#include "../systems/StoneSystem.h"
#include "../systems/ExperienceSystem.h"

SkillManager::SkillManager(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry), dialogBox(window, {"Are you sure you want to unlock this skill?"}, this->font), box(600.f, 300.f, sf::Vector2f(350.f, 200.f), this->font)
{
    this->font.loadFromFile(ASSETS_PATH + std::string("fonts/font.ttf"));

    this->dialogBox.setState(GUIDialogBoxState::Hidden);
    
    this->initializeFirstSkill();
    this->initBox();
}

void SkillManager::update()
{
    this->updateBox();
    this->box.update();
    //TODO: deltaTime
    for (auto& star : this->activeStars)
        star->update(0.016f);

    for (auto& skill : this->skills)
        skill->update();

    if (this->dialogBox.getState() != GUIDialogBoxState::Hidden)
        this->dialogBox.update();
}

void SkillManager::draw()
{
    for (auto& star : this->activeStars)
        this->window.draw(*star);

    for (auto& skill : this->skills)
        skill->draw();

    sf::View view = window.getView();

    this->window.setView(this->window.getDefaultView());

    if (this->dialogBox.getState() != GUIDialogBoxState::Hidden)
        this->dialogBox.draw();

    this->window.draw(this->box);

    this->window.setView(view);
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
        skill.multiLevel,
        skill.descriptions,
        skill.textureName,
        skill.hoverTextureName,
        skill.activeTextureName,
        skill.callbacks,
        skill.requirements,
        skill.skillsToUnlock,
        skill.maxLevel,
        skill.currentLevel,
        this->activeStars
    ));  
}

void SkillManager::initializeFirstSkill()
{
    this->addSkill(OrionProtocol);
}

void SkillManager::initBox()
{
    this->box.addText("Skill Points: " + std::to_string(ExperienceSystem::getSkillPoints(this->registry)));
    this->box.addText("Orange Stones: "+ std::to_string(StoneSystem::getStoneNumber(this->registry, std::string("Orange Stone"))));
    this->box.addText("Green Stones: " + std::to_string(StoneSystem::getStoneNumber(this->registry, std::string("Green Stone"))));
}

void SkillManager::updateBox()
{
    this->box.clearTexts();
    this->initBox();
}