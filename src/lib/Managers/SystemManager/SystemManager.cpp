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
    window(window), registry(registry), event(event), backgroundManager(registry), particleSystem(registry), gameView( window.getDefaultView() )
{
}

void SystemManager::startNewGame()
{   
    this->registry.clear();

    this->questSystem.clear();
    this->particleSystem.clear();
    this->enemyGroupSystem.clear();
    this->backgroundManager.clear();

    this->backgroundManager.initialize();

    this->initializeZoom();
    this->executeInitializationSystems();

    SkillManager::getInstance( this->registry ).clear();
    SkillManager::getInstance( this->registry ).initializeFirstSkill();

    NotifySystem::clearNotifications();

    MovementSystem::clearMovementBoost();

    //TODO: Only for testing
    this->questSystem.addRandomQuest(this->registry, "First Random Quest");
    this->questSystem.addRandomQuest(this->registry, "Second Random Quest");
    this->questSystem.addRandomQuest(this->registry, "Third Random Quest");
}

void SystemManager::executeInitializationSystems()
{
    ProceduralGenerationSystem::Initialize(std::rand());
    PlayerInitializationSystem::initializePlayer(this->registry);
    MusicSystem::initializeMusic();
}

void SystemManager::executeEventSystems()
{
    if ( SceneManager::getInstance().getCurrentScene() == Scene::Game )
    {
        if ( !this->slowMotion )
            InputSystem::processInput( this->registry, this->event );
    }

    if ( const auto* mouseWheel = this->event.getIf< sf::Event::MouseWheelScrolled >() )
    {
        const float scale = 1.0f / ConfigManager::getInstance().getScale();

        if ( mouseWheel->delta > 0 && this->zoomFactorTarget > scale )
        {
            this->zoomFactorTarget -= 0.05f * scale;
            if ( this->zoomFactorTarget < scale )
                this->zoomFactorTarget = scale;
        }
        else if ( mouseWheel->delta < 0 && this->zoomFactorTarget < scale * 2.f )
        {
            this->zoomFactorTarget += 0.05f * scale;
            if ( this->zoomFactorTarget > scale * 2.f )
                this->zoomFactorTarget = scale * 2.f;
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

void SystemManager::executeUpdateSystems( sf::Time deltaTime, const Window& window )
{
    if ( SceneManager::getInstance().getCurrentScene() == Scene::Game )
    {
        if ( SceneManager::getInstance().isGameStarted() )
        {
            Mouse::MouseState uiMouse = window.getMouseState();
            NotifySystem::update( deltaTime, uiMouse );

            if ( !NotifySystem::isDialogBoxActive() )
            {
                this->updateZoomFactor( deltaTime );

                this->gameView = this->window.getDefaultView();
                CameraSystem::updateCamera( this->gameView, this->registry, this->zoomFactor );

                this->window.setView( this->gameView );
                Mouse::MouseState gameMouse = window.getMouseState();

                if ( this->slowMotion || this->slowMotionFactor != 1.0f )
                {
                    TimeControlSystem::updateSlowMotion( this->slowMotionFactor, this->slowMotion, SLOW_MOTION_SPEED,
                                                         TARGET_SLOW_MOTION_FACTOR, deltaTime.asSeconds() );
                    deltaTime *= this->slowMotionFactor;
                }

                if ( !this->slowMotion )
                {
                    RotateTowardsMouseSystem::rotateTowardsMouse( this->registry, gameMouse, deltaTime );
                }

                backgroundManager.update();
                RemovalSystem::update( this->registry );
                this->enemyGroupSystem.updateEnemyGroup( this->registry );

                WaypointsMovementSystem::updateWaypoints( this->registry, deltaTime );
                CooldownSystem::updateCooldowns( this->registry, deltaTime );
                EnergySystem::updateEnergy( this->registry, deltaTime );
                WeaponsSystem::updateWeaponCooldown( this->registry, deltaTime );
                EntityStateSystem::updateEntityState( this->registry, deltaTime );
                ShootingSystem::shoot( this->registry, deltaTime, gameMouse );
                BulletSystem::updateShurikenBullet( this->registry, deltaTime );
                AccelerationSystem::accelerate( this->registry, deltaTime );
                MovementSystem::updateMovement( this->registry, deltaTime );
                ShieldSystem::updateShield( this->registry, deltaTime );
                HealthSystem::updateHealth( this->registry );
                DropSystem::updateDrop( this->registry, deltaTime );
                this->particleSystem.update( deltaTime );
                CollisionSystem::updateCollisionBoxes( this->registry );
                CollisionSystem::checkCollisions( this->registry );
                PointSystem::update( this->registry, deltaTime );
                InfoSystem::update( this->registry, deltaTime );

                this->questSystem.update( this->registry, deltaTime );

                if ( this->debugMode )
                {
                    DebugSystem::update( this->registry, this->window );
                }
            }
        }
        else
        {
            this->startNewGame();
        }
    }
}

void SystemManager::executeRenderSystems( Window& window )
{
    if (SceneManager::getInstance().getCurrentScene() == Scene::Game)
    {
        this->window.setView( this->gameView );

        backgroundManager.draw( window );
        this->particleSystem.draw(this->window);
        RenderSystem::renderEntities(this->window, this->registry);
        InfoSystem::draw(this->registry, this->window);
        if (this->debugMode)
        {
            DebugSystem::renderCollisionBoxes(this->registry, this->window);
            DebugSystem::renderAttackRangeCircles(this->registry, this->window);
            DebugSystem::renderBackgroundTilesFrame(this->registry, this->window, backgroundManager.getBackgroundTiles());
        }

        this->window.setView( this->window.getDefaultView() );

        NotifySystem::draw( window );
    }
}