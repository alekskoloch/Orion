#include "SystemManager.h"

#include "../systems/PlayerInitializationSystem.h"
#include "../systems/EnemyInitializationSystem.h"

#include "../systems/InputSystem.h"

#include "../systems/CollisionSystem.h"
#include "../systems/RotateTowardsMouseSystem.h"
#include "../systems/WaypointsMovementSystem.h"
#include "../systems/ShootingSystem.h"
#include "../systems/AccelerationSystem.h"
#include "../systems/MovementSystem.h"

#include "../systems/RenderSystem.h"

#include "../utils/MathOperations.h"

#include "../systems/TimeControlSystem.h"

SystemManager::SystemManager(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry) {}

void SystemManager::executeInitializationSystems()
{
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
        TimeControlSystem::updateSlowMotion(this->slowMotionFactor, this->slowMotion, this->slowMotionSpeed, deltaTime.asSeconds());
        deltaTime *= this->slowMotionFactor;
    }

    if (!this->slowMotion)
        RotateTowardsMouseSystem::rotateTowardsMouse(this->registry, deltaTime, this->window);

    WaypointsMovementSystem::updateWaypoints(this->registry, deltaTime);
    ShootingSystem::shoot(this->registry, deltaTime);
    AccelerationSystem::accelerate(this->registry, deltaTime);
    MovementSystem::updateMovement(this->registry, deltaTime);
    CollisionSystem::updateCollisionBoxes(this->registry);      
    CollisionSystem::checkCollisions(this->registry);
}

void SystemManager::executeRenderSystems()
{
    RenderSystem::renderEntities(this->window, this->registry);
}