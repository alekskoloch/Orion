#include "GUIManager.h"
#include "pch.h"


#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "SceneManager.h"

#include "ShieldSystem.h"
#include "WeaponsSystem.h"

#include "NotifySystem.h"


GUIManager::GUIManager( sf::RenderWindow& window, entt::registry& registry, sf::Event& event, std::vector< Quest >& quests )
    : window( window ), registry( registry ), event( event ), quickMenu( registry ), energyBar( registry ),
      minimap( registry, quests ), skillTreeGUI( registry ), weaponTile( registry ), shieldTile( registry ), moneyBar( registry ),
      expInfo( registry ), journal( quests ), shaderTexture( window.getSize() ), shaderSprite( shaderTexture )
{
    this->initializeShader();
}

void GUIManager::processInput()
{
    if ( !this->quickMenuActive )
    {
        this->journal.processInput( this->event );
    }
}

void GUIManager::update( sf::Time deltaTime, const Mouse::MouseState& mouseState )
{
    if ( SceneManager::getInstance().getCurrentScene() == Scene::Game )
    {
        if ( !SceneManager::getInstance().isGameStarted() )
        {
            // TODO: This is horrible, temporary solution
            this->weaponTile.clear();
            this->shieldTile.clear();
            SceneManager::getInstance().setGameStarted( true );
        }
        else
        {
            NotifySystem::update( deltaTime, mouseState );
        }

        // TODO: this solution is temporary, change of scenes should be done in a different way
        if ( this->quitTimer < 0.2f )
        {
            this->readyToQuit = false;
            this->quitTimer += deltaTime.asSeconds();
        }
        else
            this->readyToQuit = true;

        if ( this->quickMenuActive )
        {
            this->quickMenu.update( mouseState );
        }

        if ( this->journal.isOpened() )
        {
            this->pauseFromGUI = true;
            this->journal.update( mouseState, deltaTime );
        }
        else
        {
            this->pauseFromGUI = false;

            this->energyBar.update( deltaTime );
            this->minimap.update();
            this->weaponTile.update();
            this->shieldTile.update();
            this->moneyBar.update();
            this->expInfo.update();
        }

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) && this->readyToQuit )
        {
            SceneManager::getInstance().setCurrentScene( Scene::MainMenu );
        }
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::SkillTree )
    {
        const sf::View& currentSkillView = this->skillTreeGUI.getView();

        sf::Vector2i mousePixelPos =
            sf::Vector2i( static_cast< int >( mouseState.screenPosition.x ), static_cast< int >( mouseState.screenPosition.y ) );

        sf::Vector2f mouseWorldPos = this->window.mapPixelToCoords( mousePixelPos, currentSkillView );

        Mouse::MouseState skillTreeMouseState = mouseState;
        skillTreeMouseState.worldPosition = { mouseWorldPos.x, mouseWorldPos.y };

        this->skillTreeGUI.update( deltaTime, skillTreeMouseState );

        this->quitTimer = 0.F;
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::MainMenu )
    {
        this->mainMenu.update( mouseState, deltaTime );
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::Settings )
    {
        this->settings.update( mouseState, deltaTime );
    }
}

void GUIManager::draw( Window& window )
{
    if ( SceneManager::getInstance().getCurrentScene() == Scene::Game )
    {
        this->energyBar.draw( window );
        this->minimap.draw( window );
        this->expInfo.draw( window );
        this->moneyBar.draw( window );
        this->shieldTile.draw( window );
        this->weaponTile.draw( window );

        if ( this->journal.isOpened() )
        {
            this->journal.draw( window );
        }

        if ( this->quickMenuActive )
        {
            this->shaderTexture.update( this->window );
            this->window.draw( this->shaderSprite, &this->shader );

            this->quickMenu.draw( window );
        }
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::SkillTree )
    {
        this->skillTreeGUI.draw( window );
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::MainMenu )
    {
        this->mainMenu.draw( window );
    }
    else if ( SceneManager::getInstance().getCurrentScene() == Scene::Settings )
    {
        this->settings.draw( window );
    }
}

void GUIManager::toggleQuickMenu( bool value )
{
    this->quickMenuActive = value;
    // TODO: temporary solution for changing weapon
    if ( !this->quickMenuActive )
    {
        switch ( this->quickMenu.getSelectedTile() )
        {
        case 1:
            WeaponsSystem::changeWeapon( this->registry, Weapons::RedWeapon );
            this->weaponTile.setWeaponTexture( "red_weapon" );
            break;
        case 2:
            WeaponsSystem::changeWeapon( this->registry, Weapons::BlueWeapon );
            this->weaponTile.setWeaponTexture( "blue_weapon" );
            break;
        case 3:
            ShieldSystem::changeShield( this->registry, basicShield );
            this->shieldTile.setShieldTexture( "basic_shield_ico" );
            break;
        case 4:
            ShieldSystem::changeShield( this->registry, advancedShield );
            this->shieldTile.setShieldTexture( "advanced_shield_ico" );
            break;
        case 5:
            WeaponsSystem::changeWeapon( this->registry, Weapons::QuadWeapon );
            this->weaponTile.setWeaponTexture( "quad_weapon" );
            break;
        case 9:
            SceneManager::getInstance().setCurrentScene( Scene::SkillTree );
            break;
        default:
            break;
        }
    }
}

void GUIManager::initializeShader()
{
    if ( !this->shader.loadFromFile( ASSETS_PATH + std::string( "shader.frag" ), sf::Shader::Type::Fragment ) )
    {
        throw( "ERROR " );
    }

    this->shaderTexture.resize( this->window.getSize() );

    this->shaderSprite.setTexture( this->shaderTexture );
}