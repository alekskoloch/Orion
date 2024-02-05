#pragma once

#include "../pch.h"

#include "Skill.h"

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

    void update(sf::Time& deltaTime);
    void draw();

    void unlockSkills(std::vector<std::string> skillsToUnlock);
    void addSkill(std::string skillName);

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
    
    void loadSkillFromConfig(std::string skillName, std::ifstream& configFile);
    std::vector<std::pair<SkillType, float>> loadOnActivateFunctions(std::ifstream& configFile, nlohmann::json skillJson);
};