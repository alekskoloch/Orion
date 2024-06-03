#include "pch.h"
#include "SystemManager.h"

#include "ProceduralGenerationSystem.h"

#include "PlayerInitializationSystem.h"
#include "MusicSystem.h"

#include "InputSystem.h"

#include "CollisionSystem.h"
#include "CooldownSystem.h"
#include "EnergySystem.h"
#include "RotateTowardsMouseSystem.h"
#include "WaypointsMovementSystem.h"
#include "EntityStateSystem.h"
#include "ShieldSystem.h"
#include "ShootingSystem.h"
#include "BulletSystem.h"
#include "AccelerationSystem.h"
#include "MovementSystem.h"
#include "WeaponsSystem.h"
#include "HealthSystem.h"
#include "CameraSystem.h"
#include "DropSystem.h"
#include "RemovalSystem.h"
#include "InfoSystem.h"
#include "PointSystem.h"
#include "NotifySystem.h"
#include "QuestSystem.h"
#include "TimeControlSystem.h"


#include "RenderSystem.h"
#include "DebugSystem.h"

#include "MathOperations.h"


SystemManager::SystemManager(sf::RenderWindow& window, entt::registry& registry, sf::Event& event) :
    window(window), registry(registry), event(event), backgroundManager(registry, window), particleSystem(registry), tutorialSystem(window, registry)
{

}

void SystemManager::startNewGame()
{   
    this->registry.clear();

    this->questSystem.clear();
    this->particleSystem.clear();
    this->enemyGroupSystem.clear();
    this->backgroundManager.clear();
    this->tutorialSystem.clear();

    this->initializeZoom();
    this->executeInitializationSystems();

    SkillManager::getInstance(this->window, this->registry).clear();
    SkillManager::getInstance(this->window, this->registry).initializeFirstSkill();

    NotifySystem::clearNotifications();

    MovementSystem::clearMovementBoost();
}

void SystemManager::executeInitializationSystems()
{
    ProceduralGenerationSystem::Initialize(std::rand());
    PlayerInitializationSystem::initializePlayer(this->registry);
    MusicSystem::initializeMusic();
}

void SystemManager::executeEventSystems()
{
    if (SceneManager::getInstance().getCurrentScene() == Scene::Game)
    {
        if (!this->slowMotion)
            InputSystem::processInput(this->registry, this->event);
    }

    //TODO: Refactor this
    if (this->event.type == sf::Event::MouseWheelScrolled)
    {
        if (this->event.mouseWheelScroll.delta > 0 && this->zoomFactorTarget > 1 / ConfigManager::getInstance().getScale())
        {
            this->zoomFactorTarget -= 0.05f * (1 / ConfigManager::getInstance().getScale());
            if (this->zoomFactorTarget < 1 / ConfigManager::getInstance().getScale())
                this->zoomFactorTarget = 1 / ConfigManager::getInstance().getScale();
        }
        else if (this->event.mouseWheelScroll.delta < 0 && this->zoomFactorTarget < (1 / ConfigManager::getInstance().getScale()) * 2.f)
        {
            this->zoomFactorTarget += 0.05f * (1 / ConfigManager::getInstance().getScale());
            if (this->zoomFactorTarget > (1 / ConfigManager::getInstance().getScale()) * 2.f)
                this->zoomFactorTarget = (1 / ConfigManager::getInstance().getScale()) * 2.f;
        }
    }
}

void SystemManager::initializeZoom()
{
    this->zoomFactor = 1 / ConfigManager::getInstance().getScale();
    this->zoomFactorTarget = this->zoomFactor;
}

void SystemManager::updateZoomFactor(sf::Time deltaTime)
{
    if (this->zoomFactor != this->zoomFactorTarget)
    {
        float zoomFactorDelta = this->zoomFactorTarget - this->zoomFactor;
        float zoomFactorChange = zoomFactorDelta * deltaTime.asSeconds() * 5.f;
        this->zoomFactor += zoomFactorChange;
    }
}

void SystemManager::executeUpdateSystems(sf::Time deltaTime)
{
    if (SceneManager::getInstance().getCurrentScene() == Scene::Game)
    {
        if (SceneManager::getInstance().isGameStarted())
        {
            NotifySystem::update(deltaTime);

            //TODO: should be handled by GameState
            if (!NotifySystem::isDialogBoxActive())
            {
                this->updateZoomFactor(deltaTime);

                if (this->slowMotion || this->slowMotionFactor != 1.0f)
                {
                    TimeControlSystem::updateSlowMotion(this->slowMotionFactor, this->slowMotion, SLOW_MOTION_SPEED, TARGET_SLOW_MOTION_FACTOR, deltaTime.asSeconds());
                    deltaTime *= this->slowMotionFactor;
                }

                if (!this->slowMotion)
                    RotateTowardsMouseSystem::rotateTowardsMouse(this->registry, deltaTime, this->window);

                backgroundManager.update();

                RemovalSystem::update(this->registry);
                this->enemyGroupSystem.updateEnemyGroup(this->registry);
                
                WaypointsMovementSystem::updateWaypoints(this->registry, deltaTime);
                CooldownSystem::updateCooldowns(this->registry, deltaTime);
                EnergySystem::updateEnergy(this->registry, deltaTime);
                WeaponsSystem::updateWeaponCooldown(this->registry, deltaTime);
                EntityStateSystem::updateEntityState(this->registry, deltaTime);
                ShootingSystem::shoot(this->registry, deltaTime, this->window);
                BulletSystem::updateShurikenBullet(this->registry, deltaTime);
                AccelerationSystem::accelerate(this->registry, deltaTime);
                MovementSystem::updateMovement(this->registry, deltaTime);
                ShieldSystem::updateShield(this->registry, deltaTime);
                HealthSystem::updateHealth(this->registry);
                DropSystem::updateDrop(this->registry, deltaTime);
                this->particleSystem.update(deltaTime);
                CollisionSystem::updateCollisionBoxes(this->registry);      
                CollisionSystem::checkCollisions(this->registry, this->window);
                PointSystem::update(this->registry, deltaTime);
                InfoSystem::update(this->registry, deltaTime);
                this->tutorialSystem.update(deltaTime, this->window, this->questSystem);

                this->questSystem.update(this->registry, deltaTime);

                if (this->debugMode)
                    DebugSystem::update(this->registry, this->window);
            }
        }
        else
        {
            this->startNewGame();
        }
    }
}

void SystemManager::executeRenderSystems()
{
    if (SceneManager::getInstance().getCurrentScene() == Scene::Game)
    {
        CameraSystem::setPlayerCamera(this->registry, this->window);
        CameraSystem::setZoomFactor(this->zoomFactor, this->window);
        
        backgroundManager.draw();
        
        this->particleSystem.draw(this->window);
        RenderSystem::renderEntities(this->window, this->registry);
        InfoSystem::draw(this->registry, this->window);
        if (this->debugMode)
        {
            DebugSystem::renderCollisionBoxes(this->registry, this->window);
            DebugSystem::renderAttackRangeCircles(this->registry, this->window);
            DebugSystem::renderBackgroundTilesFrame(this->registry, this->window, backgroundManager.getBackgroundTiles());
        }

        CameraSystem::setDefaultCamera(this->window);

        NotifySystem::draw(this->window);
    }
}