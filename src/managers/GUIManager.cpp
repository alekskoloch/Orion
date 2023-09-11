#include "GUIManager.h"

#include "../systems/WeaponsSystem.h"
#include "../systems/ShieldSystem.h"

#include "../managers/TextureManager.h"
#include "../systems/PlayerInitializationSystem.h"
#include "../schema/WeaponsSchema.h"

GUIManager::GUIManager(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry), quickMenu(window, registry), energyBar(window, registry), minimap(window, registry)
{
    this->initializeShader();
}

void GUIManager::update()
{
    if (this->quickMenuActive)
        this->quickMenu.update();

    this->energyBar.update();
    this->minimap.update();
}

void GUIManager::draw()
{
    this->energyBar.draw();
    this->minimap.draw();
    
    if (this->quickMenuActive)
    {
        this->shaderTexture.update(this->window);
        this->window.draw(this->shaderSprite, &this->shader);

        this->quickMenu.draw();
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
            break;
        case 2:
            WeaponsSystem::changeWeapon(this->registry, blueWeapon);
            break;
        case 3:
            ShieldSystem::changeShield(this->registry, basicShield);
            break;
        case 4:
            ShieldSystem::changeShield(this->registry, advancedShield);
            break;
        case 9:
            //TODO: Open skill tree
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