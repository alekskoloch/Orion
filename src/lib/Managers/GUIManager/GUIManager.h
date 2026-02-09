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
#include "InputContext.hpp"

using GameWidget = GUIBaseElement< GUIEnergyBar, GUIMinimap, GUIMoneyBar, GUIExpInfo, GUIJournal, GUIQuickMenu, GUIWeaponTile, GUIShieldTile >;

class GUIManager
{
public:
    GUIManager( entt::registry& registry, sf::Event& event, std::vector<Quest>& quests );

    void processInput( const InputContext& inputContext );
    void update( sf::Time deltaTime, const Mouse::MouseState& mousePosition );
    void draw( Window& window );

    auto getSkillTreeView() const -> const sf::View& {
        return this->skillTreeGUI.getView();
    }

    bool pause() const { return this->pauseFromGUI; }
    bool slowMotion() const { return this->slowMotionFromGUI; }
private:
    entt::registry& registry;
    sf::Event& event;

    std::vector< GameWidget > m_widgets;

    GUIMainMenu mainMenu;
    GUISkillTree skillTreeGUI;
    GUISettings settings;

    bool quickMenuActive = false;
    bool pauseFromGUI = false;
    bool slowMotionFromGUI = false;
    bool readyToQuit = true;
    float quitTimer = 0.f;

    bool tilesReady = false;
};