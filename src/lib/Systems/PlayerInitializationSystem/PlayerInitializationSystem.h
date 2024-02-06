#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"


#include "WeaponsSystem.h"
#include "ShieldSystem.h"

#include "position.h"
#include "renderable.h"
#include "input.h"
#include "speed.h"
#include "velocity.h"
#include "acceleration.h"
#include "movementBoost.h"
#include "rotationTowardsMouse.h"
#include "shield.h"
#include "skills.h"
#include "experience.h"
#include "stoneInventory.h"
#include "moneyInventory.h"
#include "cooldown.h"
#include "energy.h"
#include "player.h"

#include "GraphicsOperations.h"

#include "WeaponsSchema.h"

class PlayerInitializationSystem
{
public:
    static void initializePlayer(entt::registry& registry);
};