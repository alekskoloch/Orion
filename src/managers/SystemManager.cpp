#include "SystemManager.h"

#include "../systems/PlayerInitializationSystem.h"
#include "../systems/EnemyInitializationSystem.h"

#include "../systems/InputSystem.h"

#include "../systems/RotateTowardsMouseSystem.h"
#include "../systems/WaypointsMovementSystem.h"
#include "../systems/ShootingSystem.h"
#include "../systems/AccelerationSystem.h"
#include "../systems/MovementSystem.h"

#include "../systems/RenderSystem.h"

#include "../utils/MathOperations.h"

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
        this->updateSlowMotion(deltaTime);
    }
    deltaTime *= this->slowMotionFactor;

    if (!this->slowMotion)
        RotateTowardsMouseSystem::rotateTowardsMouse(this->registry, deltaTime, this->window);
        
    WaypointsMovementSystem::updateWaypoints(this->registry, deltaTime);
    ShootingSystem::shoot(this->registry, deltaTime);
    AccelerationSystem::accelerate(this->registry, deltaTime);
    MovementSystem::updateMovement(this->registry, deltaTime);
}

void SystemManager::executeRenderSystems()
{
    RenderSystem::renderEntities(this->window, this->registry);
}

void SystemManager::updateSlowMotion(sf::Time deltaTime)
{
    float targetFactor = slowMotion ? targetSlowMotionFactor : 1.0f;
    float difference = targetFactor - slowMotionFactor;
    float step = slowMotionSpeed * deltaTime.asSeconds();

    if (std::abs(difference) < step)
    {
        slowMotionFactor = targetFactor;
    } else
    {
        slowMotionFactor += std::copysign(step, difference);
    }
}