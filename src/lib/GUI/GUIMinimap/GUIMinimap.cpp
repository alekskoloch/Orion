#include "GUIMinimap.h"
#include "pch.h"


#include "ConfigManager.hpp"

#include "FontManager.h"

#include "enemy.h"
#include "player.h"
#include "position.h"
#include "renderable.h"


#include "QuestCore/Quest.h"

#include "GraphicsOperations.h"

#include "Window.hpp"

namespace
{
constexpr auto MINIMAP_SCALE_FACTOR = 20.0F;
constexpr auto MIN_ENEMY_DOT_SIZE = 2.0F;
constexpr auto MAX_ENEMY_DOT_SIZE = 5.0F;
constexpr auto MINIMAP_RADIUS = 200;
constexpr auto MINIMAP_POINT_COUNT = 100;
constexpr auto MINIMAP_COLOR_VALUE = 128;
constexpr auto MINIMAP_COLOR_ALPHA = 200;
constexpr auto MINIMAP_POSITION_X = 3340;
constexpr auto MINIMAP_POSITION_Y = 100;
} // namespace

GUIMinimap::GUIMinimap( entt::registry& registry, std::vector< Quest >& quests )
    : registry( registry ), font( FontManager::getInstance().getFont( "font" ) ), quests( quests ),
      playerMinimapSprite( CreateSprite( "PLAYER_MINIMAP_TEXTURE" ) ),
      activeQuestMinimapSprite( CreateSprite( "ACTIVE_QUEST_MINIMAP_TEXTURE" ) ), playerCoordinatesText{ this->font },
      activeQuestDescriptionText( this->font ), activeQuestTitleText( this->font ), activeQuestDistanceText( this->font )
{
    initializationMinimap();
    initializePlayerCoordinatesText();
    initializeActiveQuestText();
}

void GUIMinimap::update()
{
    // TODO: temporary solution, refactor this
    auto player = registry.view< Player, Renderable >().front();
    auto playerPosition = registry.get< Position >( player );
    auto playerRotation = registry.get< Renderable >( player ).sprite.getRotation();
    playerMinimapSprite.setRotation( playerRotation );

    mapObjects.clear();

    float maxDistance = backgroundMap.getRadius() * ( MINIMAP_SCALE_FACTOR * ( 1 / ConfigManager::getInstance().getScale() ) );

    auto enemies = registry.view< Enemy, Position >();
    for ( auto enemy : enemies )
    {
        auto enemyPosition = registry.get< Position >( enemy );

        float distance = std::sqrt( ( ( enemyPosition.position.x - playerPosition.position.x ) *
                                      ( enemyPosition.position.x - playerPosition.position.x ) ) +
                                    ( ( enemyPosition.position.y - playerPosition.position.y ) *
                                      ( enemyPosition.position.y - playerPosition.position.y ) ) );

        if ( distance <= maxDistance )
        {
            sf::Vector2f playerMinimapPosition( backgroundMap.getPosition().x + backgroundMap.getRadius(),
                                                backgroundMap.getPosition().y + backgroundMap.getRadius() );

            sf::Vector2f enemyMinimapPosition(
                playerMinimapPosition.x + ( ( enemyPosition.position.x - playerPosition.position.x ) /
                                            ( MINIMAP_SCALE_FACTOR * ( 1 / ConfigManager::getInstance().getScale() ) ) ),
                playerMinimapPosition.y + ( ( enemyPosition.position.y - playerPosition.position.y ) /
                                            ( MINIMAP_SCALE_FACTOR * ( 1 / ConfigManager::getInstance().getScale() ) ) ) );

            sf::CircleShape enemyDot;
            enemyDot.setRadius(
                std::max( MIN_ENEMY_DOT_SIZE, std::round( MAX_ENEMY_DOT_SIZE * ConfigManager::getInstance().getScale() ) ) );
            enemyDot.setFillColor( sf::Color::Red );
            enemyDot.setPosition( enemyMinimapPosition );
            mapObjects.push_back( enemyDot );
        }
    }

    this->updateQuestMarker();

    this->updatePlayerCoordinates();
    this->updateActiveQuestText();
}

void GUIMinimap::draw( Window& window )
{
    window.draw( backgroundMap );
    window.draw( playerMinimapSprite );
    for ( const auto& mapObject : mapObjects )
    {
        window.draw( mapObject );
    }

    if ( this->drawQuestMarker )
    {
        window.draw( activeQuestMinimapSprite );
    }

    this->writePlayerCoordinates( window );
    this->drawActiveQuestText( window );
}

void GUIMinimap::initializationMinimap()
{
    TextureManager::getInstance().loadTexture( "PLAYER_MINIMAP_TEXTURE", ASSETS_PATH + std::string( "playerMinimap.png" ) );
    TextureManager::getInstance().loadTexture( "ACTIVE_QUEST_MINIMAP_TEXTURE", ASSETS_PATH + std::string( "MapMarker.png" ) );

    // TODO: temporary solution, make this configurable
    backgroundMap.setRadius( MINIMAP_RADIUS * ConfigManager::getInstance().getScale() );
    backgroundMap.setPointCount( MINIMAP_POINT_COUNT );
    backgroundMap.setFillColor( sf::Color( MINIMAP_COLOR_VALUE, MINIMAP_COLOR_VALUE, MINIMAP_COLOR_VALUE, MINIMAP_COLOR_ALPHA ) );
    backgroundMap.setOutlineThickness( 4 );
    backgroundMap.setOutlineColor( sf::Color::White );
    backgroundMap.setPosition( sf::Vector2f{ MINIMAP_POSITION_X * ConfigManager::getInstance().getScale(),
                                             MINIMAP_POSITION_Y * ConfigManager::getInstance().getScale() } );

    playerMinimapSprite.setScale(
        sf::Vector2f{ ConfigManager::getInstance().getScale(), ConfigManager::getInstance().getScale() } );
    playerMinimapSprite.setPosition( sf::Vector2f{ backgroundMap.getPosition().x + backgroundMap.getRadius(),
                                                   backgroundMap.getPosition().y + backgroundMap.getRadius() } );

    activeQuestMinimapSprite.setScale(
        sf::Vector2f{ ConfigManager::getInstance().getScale(), ConfigManager::getInstance().getScale() } );
}

void GUIMinimap::initializePlayerCoordinatesText()
{
    playerCoordinatesText.setCharacterSize( static_cast< float >( 20 ) * ConfigManager::getInstance().getScale() );
    playerCoordinatesText.setFillColor( sf::Color::White );
    playerCoordinatesText.setPosition( sf::Vector2f{ backgroundMap.getPosition().x + backgroundMap.getRadius(),
                                                     backgroundMap.getPosition().y + backgroundMap.getRadius() +
                                                         backgroundMap.getRadius() +
                                                         40 * ConfigManager::getInstance().getScale() } );
}

void GUIMinimap::updatePlayerCoordinates()
{
    playerCoordinatesText.setString(
        std::to_string( (int)registry.get< Position >( registry.view< Player, Renderable >().front() ).position.x / 100 ) + "  " +
        std::to_string( (int)registry.get< Position >( registry.view< Player, Renderable >().front() ).position.y / 100 ) );
    playerCoordinatesText.setOrigin( playerCoordinatesText.getLocalBounds().getCenter() );
}

void GUIMinimap::writePlayerCoordinates( Window& window ) { window.draw( playerCoordinatesText ); }

void GUIMinimap::initializeActiveQuestText()
{
    activeQuestTitleText.setFont( this->font );
    activeQuestTitleText.setCharacterSize( 20 * ConfigManager::getInstance().getScale() );
    activeQuestTitleText.setFillColor( sf::Color::White );
    activeQuestTitleText.setPosition(
        sf::Vector2f{ playerCoordinatesText.getPosition().x, playerCoordinatesText.getPosition().y +
                                                                 playerCoordinatesText.getLocalBounds().size.y +
                                                                 40 * ConfigManager::getInstance().getScale() } );

    activeQuestDescriptionText.setFont( this->font );
    activeQuestDescriptionText.setCharacterSize( 20 * ConfigManager::getInstance().getScale() );
    activeQuestDescriptionText.setFillColor( sf::Color::White );
    activeQuestDescriptionText.setPosition(
        sf::Vector2f{ activeQuestTitleText.getPosition().x, activeQuestTitleText.getPosition().y +
                                                                activeQuestTitleText.getLocalBounds().size.y +
                                                                40 * ConfigManager::getInstance().getScale() } );

    activeQuestDistanceText.setFont( this->font );
    activeQuestDistanceText.setCharacterSize( 20 * ConfigManager::getInstance().getScale() );
    activeQuestDistanceText.setFillColor( sf::Color::White );
    activeQuestDistanceText.setPosition(
        sf::Vector2f{ activeQuestDescriptionText.getPosition().x, activeQuestDescriptionText.getPosition().y +
                                                                      activeQuestDescriptionText.getLocalBounds().size.y +
                                                                      40 * ConfigManager::getInstance().getScale() } );
}

void GUIMinimap::updateActiveQuestText()
{
    activeQuestTitleText.setString( "" );
    activeQuestDescriptionText.setString( "" );
    activeQuestDistanceText.setString( "" );

    for ( auto& quest : this->quests )
    {
        if ( quest.active && !quest.completed )
        {
            activeQuestTitleText.setString( quest.name );
            activeQuestTitleText.setOrigin( activeQuestTitleText.getLocalBounds().getCenter() );

            activeQuestDescriptionText.setString( quest.stages[ quest.currentStage ].description );
            activeQuestDescriptionText.setOrigin( activeQuestDescriptionText.getLocalBounds().getCenter() );

            int distance = (int)( quest.stages[ quest.currentStage ].condition->getTargetDistance( registry ) ) / 100;
            if ( distance > 0 )
            {
                activeQuestDistanceText.setString( "Distance: " + std::to_string( distance ) );
                activeQuestDistanceText.setOrigin( activeQuestDistanceText.getLocalBounds().getCenter() );
            }

            // assuming that only one quest can be active at a time
            break;
        }
    }
}

void GUIMinimap::drawActiveQuestText( Window& window )
{
    window.draw( activeQuestTitleText );
    window.draw( activeQuestDescriptionText );
    window.draw( activeQuestDistanceText );
}

void GUIMinimap::updateQuestMarker()
{
    this->drawQuestMarker = false;
    for ( auto& quest : this->quests )
    {
        if ( quest.active && !quest.completed )
        {
            auto questMarkerPosition = quest.stages[ quest.currentStage ].condition->getTargetPosition( registry );
            if ( questMarkerPosition != sf::Vector2f() )
            {
                this->drawQuestMarker = true;

                sf::Vector2f playerPosition = registry.get< Position >( registry.view< Player, Renderable >().front() ).position;
                sf::Vector2f playerMinimapPosition =
                    backgroundMap.getPosition() + sf::Vector2f( backgroundMap.getRadius(), backgroundMap.getRadius() );

                sf::Vector2f direction = questMarkerPosition - playerPosition;
                float distance = std::sqrt( (direction.x * direction.x) + (direction.y * direction.y) );
                float maxDistance = backgroundMap.getRadius();

                sf::Vector2f normalizedDirection = direction / distance;

                if ( distance > maxDistance * MINIMAP_SCALE_FACTOR )
                {
                    sf::Vector2f edgePosition = playerMinimapPosition + normalizedDirection * ( maxDistance - 10 );
                    activeQuestMinimapSprite.setPosition( edgePosition );
                }
                else
                {
                    sf::Vector2f questMarkerMinimapPosition = playerMinimapPosition + ( direction / MINIMAP_SCALE_FACTOR );
                    activeQuestMinimapSprite.setPosition( questMarkerMinimapPosition );
                }

                activeQuestMinimapSprite.setOrigin( activeQuestMinimapSprite.getLocalBounds().getCenter() );
            }

            break;
        }
    }
}