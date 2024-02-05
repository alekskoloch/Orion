#include "GUIManager.h"

#include "../systems/WeaponsSystem.h"
#include "../systems/ShieldSystem.h"

#include "../systems/PlayerInitializationSystem.h"
#include "../schema/WeaponsSchema.h"

#include "../utils/GraphicsOperations.h"
#include "../utils/Mouse.h"

GUIManager::GUIManager(sf::RenderWindow& window, entt::registry& registry, sf::Event& event, std::vector<Quest>& quests) : window(window), registry(registry), event(event),
    quickMenu(window, registry),
    energyBar(window, registry),
    minimap(window, registry, quests),
    skillTreeGUI(registry, window),
    weaponTile(window, registry),
    shieldTile(window, registry),
    moneyBar(window, registry),
    expInfo(window, registry),
    journal(window, registry, quests)
{
    this->initializeShader();
}

void GUIManager::processInput()
{
    if (!this->quickMenuActive)
        this->journal.processInput(this->event);
}

void GUIManager::update(sf::Time deltaTime)
{
    if (SceneManager::getInstance().getCurrentScene() == Scene::Game)
    {
        if (this->quickMenuActive)
            this->quickMenu.update();

        if (this->journal.isOpened())
        {
            this->pauseFromGUI = true;
            this->journal.update(deltaTime);
        }
        else
        {
            this->pauseFromGUI = false;
            
            this->energyBar.update(deltaTime);
            this->minimap.update();
            this->weaponTile.update();
            this->shieldTile.update();
            this->moneyBar.update();
            this->expInfo.update();
        }
    }
    else if (SceneManager::getInstance().getCurrentScene() == Scene::SkillTree)
    {
        this->skillTreeGUI.update(deltaTime);
    }
}

void GUIManager::draw()
{
    if (SceneManager::getInstance().getCurrentScene() == Scene::Game)
    {
        this->energyBar.draw();
        this->minimap.draw();
        this->expInfo.draw(this->window, sf::RenderStates::Default);
        this->moneyBar.draw(this->window, sf::RenderStates::Default);
        this->shieldTile.draw(this->window, sf::RenderStates::Default);
        this->weaponTile.draw(this->window, sf::RenderStates::Default);

        if (this->journal.isOpened())
            this->journal.draw(this->window, sf::RenderStates::Default);
        
        if (this->quickMenuActive)
        {
            this->shaderTexture.update(this->window);
            this->window.draw(this->shaderSprite, &this->shader);

            this->quickMenu.draw();
        }

    }
    else if (SceneManager::getInstance().getCurrentScene() == Scene::SkillTree)
    {
        this->skillTreeGUI.draw();
    }
}

void GUIManager::toggleQuickMenu(bool value)
{
    this->quickMenuActive = value;
    //TODO: temporary solution for changing weapon
    if (!this->quickMenuActive)
    {
        switch (this->quickMenu.getSelectedTile())
        {
        case 1:
            WeaponsSystem::changeWeapon(this->registry, Weapons::RedWeapon);
            this->weaponTile.setWeaponTexture("red_weapon");
            break;
        case 2:
            WeaponsSystem::changeWeapon(this->registry, Weapons::BlueWeapon);
            this->weaponTile.setWeaponTexture("blue_weapon");
            break;
        case 3:
            ShieldSystem::changeShield(this->registry, basicShield);
            this->shieldTile.setShieldTexture("basic_shield_ico");
            break;
        case 4:
            ShieldSystem::changeShield(this->registry, advancedShield);
            this->shieldTile.setShieldTexture("advanced_shield_ico");
            break;
        case 5:
            WeaponsSystem::changeWeapon(this->registry, Weapons::QuadWeapon);
            this->weaponTile.setWeaponTexture("quad_weapon");
            break;
        case 9:
            SceneManager::getInstance().setCurrentScene(Scene::SkillTree);
            break;
        default:
            break;
        }
    }
}

void GUIManager::initializeShader()
{
    this->shader.loadFromFile(ASSETS_PATH + std::string("shader.frag"), sf::Shader::Fragment);
    this->shaderTexture.create(this->window.getSize().x, this->window.getSize().y);
    this->shaderSprite.setTexture(this->shaderTexture);
}