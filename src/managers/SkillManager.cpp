#include "SkillManager.h"

#include "../utils/Mouse.h"
#include "../utils/GraphicsOperations.h"

const float MARGIN = 120.f;

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
                this->skillManager.unlockSkills(this->skillsToUnlock);
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



SkillManager::SkillManager(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry)
{
    this->font.loadFromFile(ASSETS_PATH + std::string("fonts/font.ttf"));
    
    this->initializeFirstSkill();
}

void SkillManager::update()
{
    for (auto& skill : this->skills)
        skill.update();
}

void SkillManager::draw()
{
    for (auto& skill : this->skills)
        skill.draw();
}

void SkillManager::unlockSkills(std::vector<SkillSchema> skillsToUnlock)
{
    for (const SkillSchema& skillSchema : skillsToUnlock)
        this->addSkill(skillSchema);
}

void SkillManager::addSkill(const SkillSchema& skill)
{
    this->skills.push_back(Skill(
        this->window,
        this->registry,
        this->font,
        *this,
        skill.position,
        skill.name,
        skill.descriptions[0],
        skill.textureName,
        skill.hoverTextureName,
        skill.activeTextureName,
        skill.callbacks[0],
        skill.skillsToUnlock
    ));
}

void SkillManager::initializeFirstSkill()
{
    this->addSkill(OrionProtocol);
}