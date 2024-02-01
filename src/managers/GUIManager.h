#pragma once

#include "../pch.h"

#include "GUIScenes/GUISkillTree.h"

#include "GUIElements/GUIQuickMenu.h"
#include "GUIElements/GUIEnergyBar.h"
#include "GUIElements/GUIMinimap.h"
#include "GUIElements/GUIWeaponTile.h"
#include "GUIElements/GUIExpInfo.h"
#include "GUIElements/GUIJournal.h"

class GUIManager
{
public:
    GUIManager(sf::RenderWindow& window, entt::registry& registry, sf::Event& event, std::vector<Quest>& quests);

    void processInput();
    void update(sf::Time deltaTime);
    void draw();

    void toggleQuickMenu(bool value);

    bool pause() const { return this->pauseFromGUI; }
private:
    entt::registry& registry;
    sf::RenderWindow& window;
    sf::Event& event;

    GUIQuickMenu quickMenu;
    GUIEnergyBar energyBar;
    GUIMinimap minimap;
    GUIMoneyBar moneyBar;
    GUIWeaponTile weaponTile;
    GUIShieldTile shieldTile;
    GUIExpInfo expInfo;
    GUIJournal journal;

    GUISkillTree skillTreeGUI;

    bool quickMenuActive = false;
    bool pauseFromGUI = false;

//TODO: move shader
    sf::Shader shader;
    sf::Texture shaderTexture;
    sf::Sprite shaderSprite;
    void initializeShader();
};