#include "pch.h"
#include "GUIMinimap.h"

GUIMinimap::GUIMinimap(sf::RenderWindow& window, entt::registry& registry, std::vector<Quest>& quests)
    : window(window), registry(registry), font(FontManager::getInstance().getFont("font")), quests(quests)
{
    initializationMinimap();
    initializePlayerCoordinatesText();
    initializeActiveQuestText();
}

void GUIMinimap::update()
{
    //TODO: temporary solution, refactor this
    auto player = registry.view<Player, Renderable>().front();
    auto playerPosition = registry.get<Position>(player);
    auto playerRotation = registry.get<Renderable>(player).sprite.getRotation();
    playerMinimapSprite.setRotation(playerRotation);

    mapObjects.clear();

    float maxDistance = backgroundMap.getRadius() * 20.0f;

    auto enemies = registry.view<Enemy, Position>();
    for (auto enemy : enemies)
    {
        auto enemyPosition = registry.get<Position>(enemy);

        float distance = std::sqrt(
            (enemyPosition.position.x - playerPosition.position.x) * (enemyPosition.position.x - playerPosition.position.x) +
            (enemyPosition.position.y - playerPosition.position.y) * (enemyPosition.position.y - playerPosition.position.y)
        );

        if (distance <= maxDistance)
        {
            sf::Vector2f playerMinimapPosition(
                backgroundMap.getPosition().x + backgroundMap.getRadius(),
                backgroundMap.getPosition().y + backgroundMap.getRadius()
            );

            sf::Vector2f enemyMinimapPosition(
                playerMinimapPosition.x + (enemyPosition.position.x - playerPosition.position.x) / 20,
                playerMinimapPosition.y + (enemyPosition.position.y - playerPosition.position.y) / 20
            );

            sf::CircleShape enemyDot;
            enemyDot.setRadius(5);
            enemyDot.setFillColor(sf::Color::Red);
            enemyDot.setPosition(enemyMinimapPosition);
            mapObjects.push_back(enemyDot);
        }
    }

    this->updateQuestMarker();

    this->updatePlayerCoordinates();
    this->updateActiveQuestText();
}

void GUIMinimap::draw()
{
    window.draw(backgroundMap);
    window.draw(playerMinimapSprite);
    for (auto mapObject : mapObjects)
        window.draw(mapObject);

    if (this->drawQuestMarker)
        window.draw(activeQuestMinimapSprite);

    this->writePlayerCoordinates();
    this->drawActiveQuestText();
}

void GUIMinimap::initializationMinimap()
{
    TextureManager::getInstance().loadTexture("PLAYER_MINIMAP_TEXTURE", ASSETS_PATH + std::string("playerMinimap.png"));
    TextureManager::getInstance().loadTexture("ACTIVE_QUEST_MINIMAP_TEXTURE", ASSETS_PATH + std::string("MapMarker.png"));

    //TODO: temporary solution, make this configurable
    backgroundMap.setRadius(200);
    backgroundMap.setPointCount(100);
    backgroundMap.setFillColor(sf::Color(128, 128, 128, 200));
    backgroundMap.setOutlineThickness(4);
    backgroundMap.setOutlineColor(sf::Color::White);
    backgroundMap.setPosition(3340, 100);

    playerMinimapSprite = CreateSprite("PLAYER_MINIMAP_TEXTURE");
    playerMinimapSprite.setPosition(backgroundMap.getPosition().x + backgroundMap.getRadius(), backgroundMap.getPosition().y + backgroundMap.getRadius());

    activeQuestMinimapSprite = CreateSprite("ACTIVE_QUEST_MINIMAP_TEXTURE");
}

void GUIMinimap::initializePlayerCoordinatesText()
{
    playerCoordinatesText.setFont(this->font);
    playerCoordinatesText.setCharacterSize(20);
    playerCoordinatesText.setFillColor(sf::Color::White);
    playerCoordinatesText.setPosition(backgroundMap.getPosition().x + backgroundMap.getRadius(), backgroundMap.getPosition().y + backgroundMap.getRadius() + backgroundMap.getRadius() + 20);
}

void GUIMinimap::updatePlayerCoordinates()
{
    playerCoordinatesText.setString(std::to_string((int)registry.get<Position>(registry.view<Player, Renderable>().front()).position.x / 100) + "  " + std::to_string((int)registry.get<Position>(registry.view<Player, Renderable>().front()).position.y / 100));
    playerCoordinatesText.setOrigin(playerCoordinatesText.getGlobalBounds().width / 2.f, playerCoordinatesText.getGlobalBounds().height / 2.f);
}

void GUIMinimap::writePlayerCoordinates()
{
    window.draw(playerCoordinatesText);
}

void GUIMinimap::initializeActiveQuestText()
{
    activeQuestTitleText.setFont(this->font);
    activeQuestTitleText.setCharacterSize(20);
    activeQuestTitleText.setFillColor(sf::Color::White);
    activeQuestTitleText.setPosition(backgroundMap.getPosition().x + backgroundMap.getRadius(), backgroundMap.getPosition().y + backgroundMap.getRadius() + backgroundMap.getRadius() + 20 + this->playerCoordinatesText.getGlobalBounds().height + 30);

    activeQuestDescriptionText.setFont(this->font);
    activeQuestDescriptionText.setCharacterSize(20);
    activeQuestDescriptionText.setFillColor(sf::Color::White);
    activeQuestDescriptionText.setPosition(backgroundMap.getPosition().x + backgroundMap.getRadius(), backgroundMap.getPosition().y + backgroundMap.getRadius() + backgroundMap.getRadius() + 40 + this->playerCoordinatesText.getGlobalBounds().height + 30);

    activeQuestDistanceText.setFont(this->font);
    activeQuestDistanceText.setCharacterSize(20);
    activeQuestDistanceText.setFillColor(sf::Color::White);
    activeQuestDistanceText.setPosition(backgroundMap.getPosition().x + backgroundMap.getRadius(), backgroundMap.getPosition().y + backgroundMap.getRadius() + backgroundMap.getRadius() + 60 + this->playerCoordinatesText.getGlobalBounds().height + 30);
}

void GUIMinimap::updateActiveQuestText()
{
    activeQuestTitleText.setString("");
    activeQuestDescriptionText.setString("");
    activeQuestDistanceText.setString("");

    for (auto& quest : this->quests)
    {
        if (quest.active && !quest.completed)
        {
            activeQuestTitleText.setString(quest.name);
            activeQuestTitleText.setOrigin(activeQuestTitleText.getGlobalBounds().width / 2.f, activeQuestTitleText.getGlobalBounds().height / 2.f);

            activeQuestDescriptionText.setString(quest.stages[quest.currentStage].description);
            activeQuestDescriptionText.setOrigin(activeQuestDescriptionText.getGlobalBounds().width / 2.f, activeQuestDescriptionText.getGlobalBounds().height / 2.f);

            int distance = (int)(quest.stages[quest.currentStage].condition->getTargetDistance(registry)) / 100;
            if (distance > 0)
            {
                activeQuestDistanceText.setString("Distance: " + std::to_string(distance));
                activeQuestDistanceText.setOrigin(activeQuestDistanceText.getGlobalBounds().width / 2.f, activeQuestDistanceText.getGlobalBounds().height / 2.f);
            }

            //assuming that only one quest can be active at a time
            break;
        }
    }
}

void GUIMinimap::drawActiveQuestText()
{
    window.draw(activeQuestTitleText);
    window.draw(activeQuestDescriptionText);
    window.draw(activeQuestDistanceText);
}

void GUIMinimap::updateQuestMarker()
{
    this->drawQuestMarker = false;
    for (auto& quest : this->quests)
    {
        if (quest.active && !quest.completed)
        {
            auto questMarkerPosition = quest.stages[quest.currentStage].condition->getTargetPosition(registry);
            if (questMarkerPosition != sf::Vector2f())
            {
                this->drawQuestMarker = true;

                sf::Vector2f playerPosition = registry.get<Position>(registry.view<Player, Renderable>().front()).position;
                sf::Vector2f playerMinimapPosition = backgroundMap.getPosition() + sf::Vector2f(backgroundMap.getRadius(), backgroundMap.getRadius());

                sf::Vector2f direction = questMarkerPosition - playerPosition;
                float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                float maxDistance = backgroundMap.getRadius();
                
                sf::Vector2f normalizedDirection = direction / distance;

                if (distance > maxDistance * 20.0f)
                {
                    sf::Vector2f edgePosition = playerMinimapPosition + normalizedDirection * (maxDistance - 10);
                    activeQuestMinimapSprite.setPosition(edgePosition);
                }
                else
                {
                    sf::Vector2f questMarkerMinimapPosition = playerMinimapPosition + (direction / 20.0f);
                    activeQuestMinimapSprite.setPosition(questMarkerMinimapPosition);
                }

                activeQuestMinimapSprite.setOrigin(activeQuestMinimapSprite.getGlobalBounds().width / 2.f, activeQuestMinimapSprite.getGlobalBounds().height / 2.f);
            }

            break;
        }
    }
}