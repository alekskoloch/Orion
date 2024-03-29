#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "SceneManager.h"

#include "ParticleSystem.h"
#include "QuestSystem.h"
#include "EnemyGroupSystem.h"

#include "BackgroundManager.h"


// make this configurable
const constexpr float TARGET_SLOW_MOTION_FACTOR = 0.05f;
const constexpr float SLOW_MOTION_SPEED = 3.f;

class SystemManager
{
public:
    SystemManager(sf::RenderWindow& window, entt::registry& registry, sf::Event& event);

    void executeInitializationSystems();
    void executeEventSystems();
    void executeUpdateSystems(sf::Time deltaTime);
    void executeRenderSystems();

    void enableSlowMotion() { this->slowMotion = true; }
    void disableSlowMotion() { this->slowMotion = false; }

    std::vector<Quest>& getQuests() { return this->questSystem.getQuests(); }

    bool debugMode = false;
private:
    sf::RenderWindow& window;
    entt::registry& registry;
    sf::Event& event;
    
    BackgroundManager backgroundManager;

    QuestSystem questSystem;
    ParticleSystem particleSystem;
    EnemyGroupSystem enemyGroupSystem;

    bool slowMotion = false;
    float slowMotionFactor = 1.0f;

    //TODO: transfer it to some system
    void initializeZoom();
    float zoomFactorTarget;
    float zoomFactor;
    void updateZoomFactor(sf::Time deltaTime);
};
