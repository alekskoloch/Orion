#pragma once

#include "GUIMainMenu.hpp"
#include "GUISkillTree.h"
#include "GUISettings.hpp"

#include "GUIQuickMenu.h"
#include "GUIEnergyBar.h"
#include "GUIMinimap.h"
#include "GUIWeaponTile.h"
#include "GUIExpInfo.h"
#include "GUIJournal.h"

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

    GUIMainMenu mainMenu;
    GUISkillTree skillTreeGUI;
    GUISettings settings;

    bool quickMenuActive = false;
    bool pauseFromGUI = false;

//TODO: move shader
    sf::Shader shader;
    sf::Texture shaderTexture;
    sf::Sprite shaderSprite;
    void initializeShader();
};