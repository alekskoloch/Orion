#include "GUIManager.h"

#include "../systems/WeaponsSystem.h"
#include "../systems/ShieldSystem.h"

#include "../managers/TextureManager.h"
#include "../managers/SceneManager.h"
#include "../systems/PlayerInitializationSystem.h"
#include "../schema/WeaponsSchema.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../utils/GraphicsOperations.h"
#include "../utils/Mouse.h"

GUIManager::GUIManager(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry),
    quickMenu(window, registry),
    energyBar(window, registry),
    minimap(window, registry),
    skillTreeGUI(registry, window),
    weaponTile(window, registry),
    shieldTile(window, registry),
    moneyBar(window, registry)
{
    this->initializeShader();
}

void GUIManager::update()
{
    if (SceneManager::getInstance().getCurrentScene() == Scene::Game)
    {
        if (this->quickMenuActive)
            this->quickMenu.update();

        this->energyBar.update();
        this->minimap.update();
        this->weaponTile.update();
        this->moneyBar.update();
    }
    else if (SceneManager::getInstance().getCurrentScene() == Scene::SkillTree)
    {
        this->skillTreeGUI.update();
    }
}

void GUIManager::draw()
{
    if (SceneManager::getInstance().getCurrentScene() == Scene::Game)
    {
        this->energyBar.draw();
        this->minimap.draw();
        this->moneyBar.draw(this->window, sf::RenderStates::Default);
        this->shieldTile.draw(this->window, sf::RenderStates::Default);
        this->weaponTile.draw(this->window, sf::RenderStates::Default);
        
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
            WeaponsSystem::changeWeapon(this->registry, redWeapon);
            this->weaponTile.setWeaponTexture("red_weapon");
            break;
        case 2:
            WeaponsSystem::changeWeapon(this->registry, blueWeapon);
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