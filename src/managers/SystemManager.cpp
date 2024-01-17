#include "SystemManager.h"

#include "../systems/ProceduralGenerationSystem.h"

#include "../systems/PlayerInitializationSystem.h"
#include "../systems/EnemyInitializationSystem.h"
#include "../systems/MusicSystem.h"

#include "../systems/InputSystem.h"

#include "../systems/CollisionSystem.h"
#include "../systems/CooldownSystem.h"
#include "../systems/EnergySystem.h"
#include "../systems/RotateTowardsMouseSystem.h"
#include "../systems/WaypointsMovementSystem.h"
#include "../systems/EntityStateSystem.h"
#include "../systems/ShieldSystem.h"
#include "../systems/ShootingSystem.h"
#include "../systems/BulletSystem.h"
#include "../systems/AccelerationSystem.h"
#include "../systems/MovementSystem.h"
#include "../systems/WeaponsSystem.h"
#include "../systems/HealthSystem.h"
#include "../systems/CameraSystem.h"
#include "../systems/DropSystem.h"
#include "../systems/RemovalSystem.h"
#include "../systems/InfoSystem.h"
#include "../systems/NotifySystem.h"

#include "../systems/RenderSystem.h"
#include "../systems/DebugSystem.h"

#include "../utils/MathOperations.h"

#include "../systems/TimeControlSystem.h"

SystemManager::SystemManager(sf::RenderWindow& window, entt::registry& registry, sf::Event& event) : window(window), registry(registry), event(event), backgroundManager(registry, window)
{
    SceneManager::getInstance().setCurrentScene(Scene::Game);
    this->executeInitializationSystems();
}

void SystemManager::executeInitializationSystems()
{
    //TODO: Seed should be random
    ProceduralGenerationSystem::Initialize(1);
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
        if (this->event.mouseWheelScroll.delta > 0 && this->zoomFactorTarget > 1.f)
        {
            this->zoomFactorTarget -= 0.05f;
            if (this->zoomFactorTarget < 1.f)
                this->zoomFactorTarget = 1.f;
        }
        else if (this->event.mouseWheelScroll.delta < 0 && this->zoomFactorTarget < 2.f)
        {
            this->zoomFactorTarget += 0.05f;
            if (this->zoomFactorTarget > 2.f)
                this->zoomFactorTarget = 2.f;
        }
    }
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
            
            WaypointsMovementSystem::updateWaypoints(this->registry, deltaTime);
            CooldownSystem::updateCooldowns(this->registry, deltaTime);
            EnergySystem::updateEnergy(this->registry, deltaTime);
            WeaponsSystem::updateWeaponCooldown(this->registry, deltaTime);
            EntityStateSystem::updateEntityState(this->registry);
            ShootingSystem::shoot(this->registry, deltaTime, this->window);
            BulletSystem::updateShurikenBullet(this->registry, deltaTime);
            DropSystem::updateDrop(this->registry, deltaTime);
            AccelerationSystem::accelerate(this->registry, deltaTime);
            MovementSystem::updateMovement(this->registry, deltaTime);
            ShieldSystem::updateShield(this->registry, deltaTime);
            HealthSystem::updateHealth(this->registry);
            CollisionSystem::updateCollisionBoxes(this->registry);      
            CollisionSystem::checkCollisions(this->registry, this->window);
            RemovalSystem::update(this->registry);
            InfoSystem::update(this->registry, deltaTime);
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