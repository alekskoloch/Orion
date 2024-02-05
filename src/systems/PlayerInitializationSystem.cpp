#include "PlayerInitializationSystem.h"

#include "TextureManager.h"
#include "../systems/WeaponsSystem.h"
#include "../systems/ShieldSystem.h"
#include "../schema/WeaponsSchema.h"

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

#include "../utils/GraphicsOperations.h"

// TODO: Change start position
static float playerStartPositionX = 0.f;
static float playerStartPositionY = 0.f;

void PlayerInitializationSystem::initializePlayer(entt::registry& registry)
{
    TextureManager::getInstance().loadTexture("PLAYER_TEXTURE", ASSETS_PATH + std::string("player.png"));

    auto player = registry.create();
    registry.emplace<Player>(player);
    registry.emplace<Energy>(player, 1000.f, 1000.f, 100.f);

    sf::Sprite sprite = CreateSprite("PLAYER_TEXTURE");
    registry.emplace<Renderable>(player, sprite);

    sf::FloatRect globalBounds = sprite.getGlobalBounds();

    globalBounds.width /= 2.f;
    globalBounds.height /= 2.f;

    registry.emplace<Collision>(player, globalBounds);

    registry.emplace<Position>(player, sf::Vector2f(playerStartPositionX, playerStartPositionY));

    registry.emplace<Input>(player);
    registry.emplace<Speed>(player, 1000.f);
    registry.emplace<Velocity>(player, sf::Vector2f(0.f, 0.f));
    registry.emplace<Acceleration>(player, 1000.f, 1000.f);
    registry.emplace<MovementBoost>(player, 1000.f, 500.f);
    registry.emplace<RotationTowardsMouse>(player, true, 600.f, 2.f);
    registry.emplace<Shield>(player);
    registry.emplace<Skills>(player);
    //TODO: Remove stones, values only for testing
    registry.emplace<Experience>(player, 1, 0, 100, 50);
    registry.emplace<StoneInventory>(player, 999, 999);
    registry.emplace<MoneyInventory>(player, 1000);

    registry.emplace<Cooldown>(player, std::unordered_map<std::string, float>{ {"specialShot", 0.f} });

    WeaponsSystem::loadWeapon(registry, Weapons::RedWeapon, player);
    ShieldSystem::changeShield(registry, basicShield);
}