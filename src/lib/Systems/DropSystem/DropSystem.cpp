#include "DropSystem.h"

void DropSystem::drop(entt::registry& registry, entt::entity& entity)
{
    auto& entityDrop = registry.get<Drop>(entity);

    //TODO: Temporary method of loading all money textures
    TextureManager::getInstance().loadTexture("MONEY_10", ASSETS_PATH + std::string("money10") + ".png");
    TextureManager::getInstance().loadTexture("MONEY_20", ASSETS_PATH + std::string("money20") + ".png");
    TextureManager::getInstance().loadTexture("MONEY_50", ASSETS_PATH + std::string("money50") + ".png");
    TextureManager::getInstance().loadTexture("MONEY_100", ASSETS_PATH + std::string("money100") + ".png");
    TextureManager::getInstance().loadTexture("MONEY_200", ASSETS_PATH + std::string("money200") + ".png");
    TextureManager::getInstance().loadTexture("MONEY_500", ASSETS_PATH + std::string("money500") + ".png");
    TextureManager::getInstance().loadTexture("MONEY_1000", ASSETS_PATH + std::string("money1000") + ".png");
    TextureManager::getInstance().loadTexture("STONE_GREEN", ASSETS_PATH + std::string("greenStone") + ".png");
    TextureManager::getInstance().loadTexture("STONE_ORANGE", ASSETS_PATH + std::string("orangeStone") + ".png");

    auto drop = registry.create();

    sf::Sprite dropSprite;
    switch (ProceduralGenerationSystem::GetRandomNumber(1, 9))
    {
        case 1:
            dropSprite = CreateSprite("MONEY_10");
            registry.emplace<DropItem>(drop, "money10");
            break;
        case 2:
            dropSprite = CreateSprite("MONEY_20");
            registry.emplace<DropItem>(drop, "money20");
            break;
        case 3:
            dropSprite = CreateSprite("MONEY_50");
            registry.emplace<DropItem>(drop, "money50");
            break;
        case 4:
            dropSprite = CreateSprite("MONEY_100");
            registry.emplace<DropItem>(drop, "money100");
            break;
        case 5:
            dropSprite = CreateSprite("MONEY_200");
            registry.emplace<DropItem>(drop, "money200");
            break;
        case 6:
            dropSprite = CreateSprite("MONEY_500");
            registry.emplace<DropItem>(drop, "money500");
            break;
        case 7:
            dropSprite = CreateSprite("MONEY_1000");
            registry.emplace<DropItem>(drop, "money1000");
            break;
        case 8:
            dropSprite = CreateSprite("STONE_GREEN");
            registry.emplace<DropItem>(drop, "stoneGreen");
            break;
        case 9:
            dropSprite = CreateSprite("STONE_ORANGE");
            registry.emplace<DropItem>(drop, "stoneOrange");
            break;
    }

    dropSprite.setScale(0.5f, 0.5f);
    registry.emplace<Renderable>(drop, dropSprite);

    auto dropPosition = registry.get<Position>(entity);
    registry.emplace<Position>(drop, dropPosition.position);

    float randomX = ProceduralGenerationSystem::GetRandomNumber(30.f, 50.f);
    float randomY = ProceduralGenerationSystem::GetRandomNumber(30.f, 50.f);

    if (ProceduralGenerationSystem::GetRandomNumber(0, 1))
        randomX *= -1;
    if (ProceduralGenerationSystem::GetRandomNumber(0, 1))
        randomY *= -1;

    registry.emplace<Velocity>(drop, sf::Vector2f(randomX, randomY));

    registry.emplace<Collision>(drop, dropSprite.getGlobalBounds());
}

void DropSystem::updateDrop(entt::registry& registry, sf::Time deltaTime)
{
    auto enemies = registry.view<Enemy, Health>();
    for (auto enemy : enemies)
    {
        if (registry.get<Health>(enemy).currentHealthValue <= 0)
        {
            DropSystem::drop(registry, enemy);
        }
    }

    auto view = registry.view<DropItem, Velocity>();
    for (auto entity : view)
    {
        auto& velocity = view.get<Velocity>(entity);

        if (velocity.velocity.x > 0)
        {
            velocity.velocity.x -= 10.f * deltaTime.asSeconds();
            if (velocity.velocity.x < 1.f)
                velocity.velocity.x = 0;

        }
        else if (velocity.velocity.x < 0)
        {
            velocity.velocity.x += 10.f * deltaTime.asSeconds();
            if (velocity.velocity.x > -1.f)
                velocity.velocity.x = 0;
        }

        if (velocity.velocity.y > 0)
        {
            velocity.velocity.y -= 10.f * deltaTime.asSeconds();
            if (velocity.velocity.y < 1.f)
                velocity.velocity.y = 0;
        }
        else if (velocity.velocity.y < 0)
        {
            velocity.velocity.y += 10.f * deltaTime.asSeconds();
            if (velocity.velocity.y > -1.f)
                velocity.velocity.y = 0;
        }
    }
}