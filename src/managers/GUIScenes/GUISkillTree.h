#pragma once

#include "../../pch.h"

#include "../../schema/SkillSchema.h"

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

class GUISkillTree
{
public:
    GUISkillTree(entt::registry& registry, sf::RenderWindow& window);

    void update();
    void draw();
private:
    entt::registry& registry;
    sf::RenderWindow& window;

    sf::Font font;

    sf::Sprite skillBackground;
    void initializeSkillBackground();

    std::vector<Skill> skills;
    void initializeSkills();
    void addSkill(const SkillSchema& skill);
};