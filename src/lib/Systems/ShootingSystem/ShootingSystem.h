#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "CameraSystem.h"
#include "EnergySystem.h"
#include "SkillSystem.h"
#include "WeaponsSystem.h"
#include "CooldownSystem.h"
#include "ProceduralGenerationSystem.h"
#include "ShieldSystem.h"
#include "BulletSystem.h"

#include "TextureManager.h"

#include "player.h"
#include "enemy.h"
#include "weapon.h"
#include "input.h"
#include "position.h"
#include "entityState.h"
#include "enemyModificator.h"

#include "MathOperations.h"
#include "GraphicsOperations.h"

class ShootingSystem
{
public:
    static void shoot(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window);

private:
    static void handleQueue(entt::registry& registry, sf::Time deltaTime, sf::RenderWindow& window);
};