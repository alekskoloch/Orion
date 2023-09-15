#pragma once

#include "../pch.h"

#include "../schema/SkillSchema.h"
#include "../components/components.h"

class Skill
{
public:
    Skill(sf::RenderWindow& window, entt::registry& registry, sf::Font& font, sf::Vector2f iconPosition, std::string name, std::string description, std::string iconTextureName, std::string iconHoverTextureName, std::string iconActiveTextureName, std::function<void(entt::registry&)> onActivate);

    void update();
    void draw();
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    void initialize();

    sf::Sprite iconSprite;
    sf::Vector2f iconPosition;

    std::string iconTextureName;
    std::string iconHoverTextureName;
    std::string iconActiveTextureName;

    void initializeText();
    void updateText();

    std::string name;
    std::string description;
    
    sf::Font& font;
    sf::Text nameText;
    sf::Text descriptionText;

    bool active = false;
    bool hover = false;

    std::function<void(entt::registry&)> onActivate;
};

class SkillManager
{
public:
    SkillManager(sf::RenderWindow& window, entt::registry& registry);

    void update();
    void draw();
private:
    sf::RenderWindow& window;
    entt::registry& registry;
    std::vector<Skill> skills;
    sf::Font font;

    void addSkill(const SkillSchema& skill);
    void initializeFirstSkill();
};