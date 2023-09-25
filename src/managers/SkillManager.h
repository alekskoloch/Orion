#pragma once

#include "../pch.h"

#include "Skill.h"

#include "../schema/SkillSchema.h"
#include "../components/components.h"

#include "GUIElements/GUIDialogBox.h"
#include "GUIElements/GUIStar.h"
#include "GUIElements/GUIBox.h"

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
    void addSkill(SkillSchema skill);

    void updateBox();
private:
    SkillManager(sf::RenderWindow& window, entt::registry& registry);

    sf::RenderWindow& window;
    entt::registry& registry;
    std::vector<std::unique_ptr<Skill>> skills;
    std::vector<std::unique_ptr<GUIStar>> activeStars;
    sf::Font font;

    GUIBox box;
    void initBox();


    GUIDialogBox dialogBox;

    void initializeFirstSkill();
};