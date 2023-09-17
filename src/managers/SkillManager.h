#pragma once

#include "../pch.h"

#include "Skill.h"

#include "../schema/SkillSchema.h"
#include "../components/components.h"

class Skill;

class SkillManager
{
public:
    SkillManager(sf::RenderWindow& window, entt::registry& registry);

    void update();
    void draw();

    void unlockSkills(std::vector<SkillSchema> skillsToUnlock);
private:
    sf::RenderWindow& window;
    entt::registry& registry;
    std::vector<Skill> skills;
    sf::Font font;

    void addSkill(const SkillSchema& skill);
    void initializeFirstSkill();
};