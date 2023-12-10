#include "SkillManager.h"

#include "../systems/StoneSystem.h"
#include "../systems/ExperienceSystem.h"

#include "../utils/StringOperations.h"

#include "SkillBuilder.h"

SkillManager::SkillManager(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry), dialogBox(window, {"Are you sure you want to unlock this skill?"}, this->font), box(600.f, 300.f, sf::Vector2f(350.f, 200.f), this->font)
{
    this->font.loadFromFile(ASSETS_PATH + std::string("fonts/font.ttf"));

    this->dialogBox.setState(GUIDialogBoxState::Hidden);
    
    this->initializeFirstSkill();
    this->initBox();
}

void SkillManager::update(sf::Time& deltaTime)
{
    this->updateBox();
    this->box.update();

    for (auto& star : this->activeStars)
        star->update(deltaTime.asSeconds());

    for (auto& skill : this->skills)
        skill->update(deltaTime);

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

void SkillManager::unlockSkills(std::vector<std::string> skillsToUnlock)
{
    for (const std::string& skillName : skillsToUnlock)
    {
        this->addSkill(skillName);
    }
}

void SkillManager::addSkill(std::string skillName)
{
    std::ifstream configFile(CONFIG_PATH + std::string("skillConfig.json"));
    if (!configFile.is_open())
        throw std::runtime_error("Could not open config file");
    else
        this->loadSkillFromConfig(skillName, configFile);
}

void SkillManager::initializeFirstSkill()
{
    this->addSkill("Orion Protocol");
}

void SkillManager::initBox()
{
    this->box.addText("Skill Points: " + std::to_string(ExperienceSystem::getSkillPoints(this->registry)));
    this->box.addText("Orange Stones: " + std::to_string(StoneSystem::getStoneNumber(this->registry, std::string("Orange Stone"))));
    this->box.addText("Green Stones: " + std::to_string(StoneSystem::getStoneNumber(this->registry, std::string("Green Stone"))));
}

void SkillManager::updateBox()
{
    this->box.clearTexts();
    this->initBox();
}

void SkillManager::loadSkillFromConfig(std::string skillName, std::ifstream& configFile)
{
    nlohmann::json configJson;
    configFile >> configJson;

    if (!configJson.contains("skills") || !configJson["skills"].is_array())
        throw std::runtime_error("Could not find skills array in config file");
    else
    {
        for (const auto& skillJson : configJson["skills"])
        {
            if (skillJson.contains("name") && skillJson["name"] == skillName)
            {   
                this->skills.push_back(
                    SkillBuilder()
                        .addWindow(this->window)
                        .addRegistry(this->registry)
                        .addDialogBox(this->dialogBox)
                        .addIconPosition(sf::Vector2f(skillJson["position"]["x"].get<float>(), skillJson["position"]["y"].get<float>()))
                        .addName(skillJson["name"].get<std::string>())
                        .addDescriptions(skillJson["descriptions"].get<std::vector<std::string>>())
                        .addIconTextureName(removeWhitespace(skillJson["name"].get<std::string>()))
                        .addOnActivateFunctions(this->loadOnActivateFunctions(configFile, skillJson))
                        .addRequirements(skillJson["requirements"].get<std::vector<RequirementType>>())
                        .addSkillsToUnlock(skillJson["skillsToUnlock"].get<std::vector<std::string>>())
                        .addMaxLevel(static_cast<unsigned int>(skillJson["descriptions"].get<std::vector<std::string>>().size()))
                        .addCurrentLevel(0)
                        .addActiveStars(this->activeStars)
                        .build()
                        );

                return;
            }
        }
    }
}

std::vector<std::pair<SkillType, float>> SkillManager::loadOnActivateFunctions(std::ifstream& configFile, nlohmann::json skillJson)
{
    std::vector<std::pair<SkillType, float>> onActivateFunctions;

    auto onActivateFunctionsJson = skillJson["callbacks"];
    for (const auto& onActivateFunctionJson : onActivateFunctionsJson)
    {
        onActivateFunctions.push_back(std::make_pair(
            onActivateFunctionJson["skillID"].get<SkillType>(),
            onActivateFunctionJson["value"].get<float>()
        ));
    }

    return onActivateFunctions;
}