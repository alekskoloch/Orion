#include "GUISkillTree.h"

#include "../../managers/TextureManager.h"

#include "../../components/components.h"
#include "../../components/tagComponents.h"

#include "../../utils/GraphicsOperations.h"
#include "../../utils/Mouse.h"

const float MARGIN = 120.f;

Skill::Skill(sf::RenderWindow& window, entt::registry& registry, sf::Font& font, sf::Vector2f iconPosition, std::string name, std::string description, std::string iconTextureName, std::string iconHoverTextureName, std::string iconActiveTextureName, std::function<void(entt::registry&)> onActivate)
 : window(window), registry(registry), font(font), iconPosition(iconPosition), name(name), description(description), iconTextureName(iconTextureName), iconHoverTextureName(iconHoverTextureName), iconActiveTextureName(iconActiveTextureName), onActivate(onActivate)
{
    this->initialize();
}

void Skill::initialize()
{
    TextureManager::getInstance().loadTexture(this->iconTextureName, ASSETS_PATH + this->iconTextureName + std::string(".png"));
    TextureManager::getInstance().loadTexture(this->iconHoverTextureName, ASSETS_PATH + this->iconHoverTextureName + std::string(".png"));
    TextureManager::getInstance().loadTexture(this->iconActiveTextureName , ASSETS_PATH + this->iconActiveTextureName + std::string(".png"));

    this->iconSprite = CreateSprite(this->iconTextureName);
    this->iconSprite.setPosition(this->iconPosition);

    this->initializeText();
}

void Skill::initializeText()
{
//TODO: get font from manager
    this->nameText.setString(this->name);
    this->nameText.setFont(this->font);
    this->nameText.setCharacterSize(30);
    this->nameText.setFillColor(sf::Color::White);
    this->nameText.setOutlineThickness(1);
    this->nameText.setOutlineColor(sf::Color::Black);

    this->descriptionText.setString(this->description);
    this->descriptionText.setFont(this->font);
    this->descriptionText.setCharacterSize(40);
    this->descriptionText.setFillColor(sf::Color::White);
    this->descriptionText.setOutlineThickness(1);
    this->descriptionText.setOutlineColor(sf::Color::Black);
}

void Skill::updateText()
{
    this->nameText.setOrigin(
        this->nameText.getGlobalBounds().width / 2.f,
        this->nameText.getGlobalBounds().height / 2.f
    );
    this->nameText.setPosition(
        this->iconSprite.getPosition().x,
        this->iconSprite.getPosition().y + MARGIN
    );

    this->descriptionText.setOrigin(
        this->descriptionText.getGlobalBounds().width / 2.f,
        this->descriptionText.getGlobalBounds().height / 2.f
    );
    this->descriptionText.setPosition(
        this->window.getSize().x / 2.f,
        this->window.getSize().y - MARGIN - this->descriptionText.getGlobalBounds().height
    );
}

void Skill::update()
{
    this->updateText();

    if (utils::isMouseOverSprite(this->iconSprite, sf::Mouse::getPosition(this->window)))
    {
        this->hover = true;
    }
    else
    {
        this->hover = false;
    }

    if (!this->active)
    {
        if (this->hover)
        {
            this->iconSprite.setTexture(TextureManager::getInstance().getTexture(this->iconHoverTextureName));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                this->iconSprite.setTexture(TextureManager::getInstance().getTexture(this->iconActiveTextureName));
                this->active = true;
                this->onActivate(this->registry);
            }
        }
        else
        {
            this->iconSprite.setTexture(TextureManager::getInstance().getTexture(this->iconTextureName));
        }
    }
}

void Skill::draw()
{
    this->window.draw(this->iconSprite);

    this->window.draw(this->nameText);

    if (this->hover)
        this->window.draw(this->descriptionText);
}

// ======================================================================================================

GUISkillTree::GUISkillTree(entt::registry& registry, sf::RenderWindow& window) : registry(registry), window(window)
{
    this->initializeSkillBackground();
    this->initializeSkills();

    this->font.loadFromFile(ASSETS_PATH + std::string("fonts/font.ttf"));
}

void GUISkillTree::update()
{
    for (auto& skill : this->skills)
        skill.update();
}

void GUISkillTree::draw()
{
    this->window.draw(this->skillBackground);

    for (auto& skill : this->skills)
        skill.draw();
}

void GUISkillTree::initializeSkillBackground()
{
    TextureManager::getInstance().loadTexture("skillBackground", ASSETS_PATH + std::string("skillBackground.png"));
    this->skillBackground.setTexture(TextureManager::getInstance().getTexture("skillBackground"));
    this->skillBackground.setPosition(0, 0);
}

void GUISkillTree::initializeSkills()
{
//TODO: skill manager
    this->addSkill(OrionProtocol);
    this->addSkill(LethalPrecision);
}

void GUISkillTree::addSkill(const SkillSchema& skill)
{
    this->skills.push_back(Skill(
        this->window,
        this->registry,
        this->font,
        skill.position,
        skill.name,
        skill.description,
        skill.textureName,
        skill.hoverTextureName,
        skill.activeTextureName,
        skill.callback
    ));
}