#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

#include "ConfigManager.hpp"

#include "GUIDialogBox.h"
#include "GUIStar.h"
#include "GUIBox.h"

#include "StoneSystem.h"
#include "ExperienceSystem.h"
#include "SkillSystem.h"

#include "StringOperations.h"

#include "GUISkill.h"
#include "SkillBuilder.h"

class GUISkill;

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

    void initializeFirstSkill();
    void clear();
private:
    SkillManager(sf::RenderWindow& window, entt::registry& registry);

    sf::RenderWindow& window;
    entt::registry& registry;
    std::vector<std::unique_ptr<GUISkill>> skills;
    std::vector<std::unique_ptr<GUIStar>> activeStars;
    sf::Font font;

    GUIBox box;
    void initBox();

    GUIDialogBox dialogBox;

    
    void loadSkillFromConfig(std::string skillName, std::ifstream& configFile);
    std::vector<std::pair<SkillType, float>> loadOnActivateFunctions(std::ifstream& configFile, nlohmann::json skillJson);
};