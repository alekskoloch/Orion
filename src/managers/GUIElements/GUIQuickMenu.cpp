#include "GUIQuickMenu.h"

#include "../../managers/TextureManager.h"

#include "../../utils/Mouse.h"
#include "../../utils/GraphicsOperations.h"

const uint8_t TILES_NUMBER = 8;
const float RADIUS = 360.f;
const float ANGLE_INCREMENT = 45.f;

GUIQuickMenu::GUIQuickMenu(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry)
{
    this->initializeQuickMenu();
}

void GUIQuickMenu::update()
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(this->window);
    this->selectedTile = 0;

    for (int i = 0; i < 8; i++)
    {
        if (utils::isMouseOverSprite(this->quickMenuTiles[i], mousePosition))
        {
            this->quickMenuTiles[i].setTexture(TextureManager::getInstance().getTexture("ACTIVE_TILE"));
            this->selectedTile = i + 1;
        }
        else
        {
            this->quickMenuTiles[i].setTexture(TextureManager::getInstance().getTexture("INACTIVE_TILE"));
        }
    }
}

void GUIQuickMenu::draw()
{
    for (auto& tile : this->quickMenuTiles)
    {
        this->window.draw(tile);
    }

    for (auto& icon : this->quickMenuIcons)
    {
        this->window.draw(icon);
    }
}

void GUIQuickMenu::initializeQuickMenu()
{
    TextureManager::getInstance().loadTexture("ACTIVE_TILE", ASSETS_PATH + std::string("tileActive.png"));
    TextureManager::getInstance().loadTexture("INACTIVE_TILE", ASSETS_PATH + std::string("tileInactive.png"));
    
    //TODO: this is a temporary solution for loading bullet ico textures
    TextureManager::getInstance().loadTexture("red_weapon", ASSETS_PATH + std::string("red_weapon.png"));
    TextureManager::getInstance().loadTexture("blue_weapon", ASSETS_PATH + std::string("blue_weapon.png"));
    TextureManager::getInstance().loadTexture("basic_shield_ico", ASSETS_PATH + std::string("basic_shield_ico.png"));
    TextureManager::getInstance().loadTexture("advanced_shield_ico", ASSETS_PATH + std::string("advanced_shield_ico.png"));

    this->quickMenuTiles.resize(TILES_NUMBER);
    this->quickMenuIcons.resize(TILES_NUMBER);

    for (int i = 0; i < TILES_NUMBER; i++)
    {
        this->quickMenuTiles[i] = CreateSprite("INACTIVE_TILE");
        this->quickMenuTiles[i].setPosition((this->window.getSize().x / 2.f) + std::sin((i * ANGLE_INCREMENT) * M_PI / 180.f) * RADIUS, (this->window.getSize().y / 2.f) - std::cos((i * ANGLE_INCREMENT) * M_PI / 180.f) * RADIUS);
        this->quickMenuTiles[i].setRotation(i * ANGLE_INCREMENT);
        
        //TODO: this is a temporary solution for loading bullet ico textures
        if (i == 0)
        {
            this->quickMenuIcons[i] = CreateSprite("red_weapon");
        }
        else if (i == 1)
        {
            this->quickMenuIcons[i] = CreateSprite("blue_weapon");
        }
        else if (i == 2)
        {
            this->quickMenuIcons[i] = CreateSprite("basic_shield_ico");
        }
        else if (i == 3)
        {
            this->quickMenuIcons[i] = CreateSprite("advanced_shield_ico");
        }

        this->quickMenuIcons[i].setPosition(this->quickMenuTiles[i].getPosition());
    }
}