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

SystemManager::SystemManager( entt::registry& registry, sf::Event& event, GameState* gameState )
    : registry( registry ), event( event ), backgroundManager( registry ), particleSystem( registry ), m_gameState( gameState )
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

    this->executeInitializationSystems();

    SkillManager::getInstance( this->registry ).clear();
    SkillManager::getInstance( this->registry ).initializeFirstSkill();

    NotifySystem::clearNotifications();

    MovementSystem::clearMovementBoost();

    m_gameInit = true;

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
    if ( *m_gameState == GameState::Game )
    {
        if ( !this->slowMotion )
            InputSystem::processInput( this->registry, this->event );
    }

    if ( const auto* mouseWheel = this->event.getIf< sf::Event::MouseWheelScrolled >() )
    {
        CameraSystem::zoomFactorChange( mouseWheel->delta );
    }
}

void SystemManager::executeUpdateSystems( sf::Time deltaTime, const Mouse::MouseState& mouseState )
{
    if ( *m_gameState == GameState::Game )
    {
        if ( m_gameInit )
        {
            if ( !NotifySystem::isDialogBoxActive() )
            {
                if ( this->slowMotion || this->slowMotionFactor != 1.0f )
                {
                    TimeControlSystem::updateSlowMotion( this->slowMotionFactor, this->slowMotion, SLOW_MOTION_SPEED,
                                                         TARGET_SLOW_MOTION_FACTOR, deltaTime.asSeconds() );
                    deltaTime *= this->slowMotionFactor;
                }

                if ( !this->slowMotion )
                {
                    RotateTowardsMouseSystem::rotateTowardsMouse( this->registry, mouseState, deltaTime );
                }

                backgroundManager.update();
                RemovalSystem::update( this->registry );
                this->enemyGroupSystem.updateEnemyGroup( this->registry );

                WaypointsMovementSystem::updateWaypoints( this->registry, deltaTime );
                CooldownSystem::updateCooldowns( this->registry, deltaTime );
                EnergySystem::updateEnergy( this->registry, deltaTime );
                WeaponsSystem::updateWeaponCooldown( this->registry, deltaTime );
                EntityStateSystem::updateEntityState( this->registry, deltaTime );
                ShootingSystem::shoot( this->registry, deltaTime, mouseState );
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
                    DebugSystem::update( this->registry, mouseState );
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
    if ( *m_gameState == GameState::Game )
    {
        backgroundManager.draw( window );
        this->particleSystem.draw( window );
        RenderSystem::renderEntities( window, this->registry );
        InfoSystem::draw( this->registry, window );

        if ( this->debugMode )
        {
            DebugSystem::renderCollisionBoxes( this->registry, window );
            DebugSystem::renderAttackRangeCircles( this->registry, window );
            DebugSystem::renderBackgroundTilesFrame( this->registry, window, backgroundManager.getBackgroundTiles() );
        }
    }
}