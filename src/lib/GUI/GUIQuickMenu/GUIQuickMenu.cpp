#include "pch.h"
#include "GUIQuickMenu.h"

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

    for (int i = 0; i < TILES_NUMBER; i++)
    {
        if (utils::isMouseOverSprite(this->quickMenuTiles[i], mousePosition))
        {
            if (this->selectedTile != i + 1)
                SoundManager::getInstance().playSound("MouseHover");
                
            this->quickMenuTiles[i].setTexture(TextureManager::getInstance().getTexture("ACTIVE_TILE"));
            this->selectedTile = i + 1;
        }
        else
        {
            this->quickMenuTiles[i].setTexture(TextureManager::getInstance().getTexture("INACTIVE_TILE"));
            if (this->selectedTile == i + 1)
                this->selectedTile = 0;
        }
    }

    if (utils::isMouseOverSprite(this->quickMenuTiles[TILES_NUMBER], mousePosition))
    {
        if (this->selectedTile != 9)
            SoundManager::getInstance().playSound("MouseHover");

        this->quickMenuTiles[TILES_NUMBER].setTexture(TextureManager::getInstance().getTexture("ACTIVE_MIDDLE_TILE"));
        this->selectedTile = 9;
    }
    else
    {
        this->quickMenuTiles[TILES_NUMBER].setTexture(TextureManager::getInstance().getTexture("INACTIVE_MIDDLE_TILE"));
        if (this->selectedTile == 9)
            this->selectedTile = 0;
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
    TextureManager::getInstance().loadTexture("ACTIVE_MIDDLE_TILE", ASSETS_PATH + std::string("middleTileActive.png"));
    TextureManager::getInstance().loadTexture("INACTIVE_MIDDLE_TILE", ASSETS_PATH + std::string("middleTileInactive.png"));

    //TODO: this is a temporary solution for loading bullet ico textures
    TextureManager::getInstance().loadTexture("red_weapon", ASSETS_PATH + std::string("red_weapon.png"));
    TextureManager::getInstance().loadTexture("blue_weapon", ASSETS_PATH + std::string("blue_weapon.png"));
    TextureManager::getInstance().loadTexture("blue_weapon", ASSETS_PATH + std::string("quad_weapon.png"));
    TextureManager::getInstance().loadTexture("basic_shield_ico", ASSETS_PATH + std::string("basic_shield_ico.png"));
    TextureManager::getInstance().loadTexture("advanced_shield_ico", ASSETS_PATH + std::string("advanced_shield_ico.png"));

    this->quickMenuTiles.resize(TILES_NUMBER + 1);
    this->quickMenuIcons.resize(TILES_NUMBER + 1);

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
        else if (i == 4)
        {
            this->quickMenuIcons[i] = CreateSprite("quad_weapon");
        }

        this->quickMenuIcons[i].setPosition(this->quickMenuTiles[i].getPosition());
    }

    this->quickMenuTiles[TILES_NUMBER] = CreateSprite("INACTIVE_MIDDLE_TILE");
    this->quickMenuTiles[TILES_NUMBER].setPosition(1920, 1080);
}