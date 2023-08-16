#include "SystemManager.h"

#include "../systems/ProceduralGenerationSystem.h"

#include "../systems/PlayerInitializationSystem.h"
#include "../systems/EnemyInitializationSystem.h"

#include "../systems/InputSystem.h"

#include "../systems/CollisionSystem.h"
#include "../systems/RotateTowardsMouseSystem.h"
#include "../systems/WaypointsMovementSystem.h"
#include "../systems/EntityStateSystem.h"
#include "../systems/ShootingSystem.h"
#include "../systems/AccelerationSystem.h"
#include "../systems/MovementSystem.h"
#include "../systems/WeaponsSystem.h"
#include "../systems/CameraSystem.h"

#include "../systems/RenderSystem.h"
#include "../systems/DebugSystem.h"

#include "../utils/MathOperations.h"

#include "../systems/TimeControlSystem.h"

SystemManager::SystemManager(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry)
{
    this->executeInitializationSystems();
}

void SystemManager::executeInitializationSystems()
{
    //TODO: Seed should be random
    ProceduralGenerationSystem::Initialize(1);
    PlayerInitializationSystem::initializePlayer(this->registry);
    EnemyInitializationSystem::initializeEnemy(this->registry);
}

void SystemManager::executeEventSystems()
{
    if (!this->slowMotion)
        InputSystem::processInput(this->registry);
}

void SystemManager::executeUpdateSystems(sf::Time deltaTime)
{
    if (this->slowMotion || this->slowMotionFactor != 1.0f)
    {
        TimeControlSystem::updateSlowMotion(this->slowMotionFactor, this->slowMotion, SLOW_MOTION_SPEED, TARGET_SLOW_MOTION_FACTOR, deltaTime.asSeconds());
        deltaTime *= this->slowMotionFactor;
    }

    if (!this->slowMotion)
        RotateTowardsMouseSystem::rotateTowardsMouse(this->registry, deltaTime, this->window);

    WaypointsMovementSystem::updateWaypoints(this->registry, deltaTime);
    WeaponsSystem::updateWeaponCooldown(this->registry, deltaTime);
    EntityStateSystem::updateEntityState(this->registry);
    ShootingSystem::shoot(this->registry, deltaTime, this->window);
    AccelerationSystem::accelerate(this->registry, deltaTime);
    MovementSystem::updateMovement(this->registry, deltaTime);
    CollisionSystem::updateCollisionBoxes(this->registry);      
    CollisionSystem::checkCollisions(this->registry);
}

void SystemManager::executeRenderSystems()
{
    CameraSystem::setPlayerCamera(this->registry, this->window);

    RenderSystem::renderEntities(this->window, this->registry);
    if (this->debugMode)
        DebugSystem::renderCollisionBoxes(this->registry, this->window);

    CameraSystem::setDefaultCamera(this->window);
}