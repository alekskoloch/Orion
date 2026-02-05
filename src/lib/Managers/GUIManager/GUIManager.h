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

#include "GUIBaseElement.h"

using GameWidget = GUIBaseElement< GUIQuickMenu, GUIEnergyBar, GUIMinimap, GUIMoneyBar, GUIWeaponTile, GUIShieldTile, GUIExpInfo,
                                   GUIJournal, GUIMainMenu, GUISkillTree, GUISettings >;

class GUIManager
{
public:
    GUIManager( entt::registry& registry, sf::Event& event, std::vector<Quest>& quests );

    void processInput();
    void update( sf::Time deltaTime, const Mouse::MouseState& mousePosition );
    void draw( Window& window );

    void toggleQuickMenu(bool value);

    auto getSkillTreeView() const -> const sf::View& {
        return this->skillTreeGUI.getView();
    }

    bool pause() const { return this->pauseFromGUI; }
private:
    entt::registry& registry;
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
    bool readyToQuit = true;
    float quitTimer = 0.f;

    bool tilesReady = false;
};