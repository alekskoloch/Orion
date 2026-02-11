#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "SceneManager.h"
#include "SkillManager.h"

#include "ParticleSystem.h"
#include "QuestSystem.h"
#include "EnemyGroupSystem.h"

#include "BackgroundManager.h"

#include "GameState.hpp"

// make this configurable
const constexpr float TARGET_SLOW_MOTION_FACTOR = 0.05f;
const constexpr float SLOW_MOTION_SPEED = 3.f;

class SystemManager
{
public:
    SystemManager( entt::registry& registry, sf::Event& event, GameState* gameState );
    void startNewGame();

    void executeInitializationSystems();
    void executeEventSystems();
    void executeUpdateSystems( sf::Time deltaTime, const Mouse::MouseState& mouseState );
    void executeRenderSystems( Window& window );

    void enableSlowMotion() { this->slowMotion = true; }
    void disableSlowMotion() { this->slowMotion = false; }

    std::vector<Quest>& getQuests() { return this->questSystem.getQuests(); }

    bool debugMode = false;
private:
    entt::registry& registry;
    sf::Event& event;

    GameState* m_gameState;

    BackgroundManager backgroundManager;

    QuestSystem questSystem;
    ParticleSystem particleSystem;
    EnemyGroupSystem enemyGroupSystem;

    bool slowMotion = false;
    float slowMotionFactor = 1.0f;
};
