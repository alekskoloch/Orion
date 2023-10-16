#pragma once

#include "../pch.h"

#include "GUIScenes/GUISkillTree.h"

#include "GUIElements/GUIQuickMenu.h"
#include "GUIElements/GUIEnergyBar.h"
#include "GUIElements/GUIMinimap.h"
#include "GUIElements/GUIWeaponTile.h"

class GUIManager
{
public:
    GUIManager(sf::RenderWindow& window, entt::registry& registry);

    void update();
    void draw();

    void toggleQuickMenu(bool value);

private:
    entt::registry& registry;
    sf::RenderWindow& window;

    GUIQuickMenu quickMenu;
    GUIEnergyBar energyBar;
    GUIMinimap minimap;
    GUIWeaponTile weaponTile;
    GUIShieldTile shieldTile;

    GUISkillTree skillTreeGUI;

    bool quickMenuActive = false;

//TODO: move shader
    sf::Shader shader;
    sf::Texture shaderTexture;
    sf::Sprite shaderSprite;
    void initializeShader();
};