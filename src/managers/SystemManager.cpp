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

SystemManager::SystemManager(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry) {}

void SystemManager::executeInitializationSystems()
{
    PlayerInitializationSystem::initializePlayer(this->registry);
    EnemyInitializationSystem::initializeEnemy(this->registry);
}

void SystemManager::executeEventSystems()
{
    InputSystem::processInput(this->registry);
}

void SystemManager::executeUpdateSystems(sf::Time deltaTime)
{
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