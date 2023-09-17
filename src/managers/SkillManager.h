#pragma once

#include "../pch.h"

#include "Skill.h"

#include "../schema/SkillSchema.h"
#include "../components/components.h"

class Skill;

class SkillManager
{
public:
    static SkillManager& getInstance(sf::RenderWindow& window, entt::registry& registry)
    {
        static SkillManager instance(window, registry);
        return instance;
    }

    SkillManager(const SkillManager&) = delete;
    void operator=(const SkillManager&) = delete;

    void update();
    void draw();

    void unlockSkills(std::vector<SkillSchema> skillsToUnlock);

private:
    SkillManager(sf::RenderWindow& window, entt::registry& registry);

    sf::RenderWindow& window;
    entt::registry& registry;
    std::vector<Skill> skills;
    sf::Font font;

    void addSkill(const SkillSchema& skill);
    void initializeFirstSkill();
};