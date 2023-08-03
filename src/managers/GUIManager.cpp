#include "GUIManager.h"

#include <cmath>

#include "../systems/WeaponsSystem.h"

#include "../managers/TextureManager.h"
#include "../systems/PlayerInitializationSystem.h"
#include "../schema/WeaponsSchema.h"

//TODO: move to utils / optimize it
bool isMouseOverSprite(const sf::Sprite& sprite, const sf::Vector2i& mousePosition)
{
    sf::Vector2f localPosition = sprite.getInverseTransform().transformPoint(static_cast<sf::Vector2f>(mousePosition));
    sf::IntRect textureRect = sprite.getTextureRect();

    if (localPosition.x >= 0 && localPosition.y >= 0 && localPosition.x < textureRect.width && localPosition.y < textureRect.height)
    {
        const sf::Texture* texture = sprite.getTexture();
        sf::Image image = texture->copyToImage();

        return image.getPixel(static_cast<unsigned int>(localPosition.x) + textureRect.left,
                              static_cast<unsigned int>(localPosition.y) + textureRect.top).a > 0;
    }

    return false;
}

GUIManager::GUIManager(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry)
{
    this->initializeQuickMenu();
    this->initializeShader();
}

void GUIManager::update()
{
    if (this->quickMenuActive)
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(this->window);
        this->selectedTile = 0;

        for (int i = 0; i < 8; i++)
        {
            if (isMouseOverSprite(this->quickMenuTiles[i], mousePosition))
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
}

void GUIManager::draw()
{
    if (this->quickMenuActive)
    {
        this->shaderTexture.update(this->window);
        this->window.draw(this->shaderSprite, &this->shader);

        for (auto& tile : this->quickMenuTiles)
        {
            this->window.draw(tile);
        }

        for (auto& icon : this->quickMenuIcons)
        {
            this->window.draw(icon);
        }
    }
}

void GUIManager::toggleQuickMenu(bool value)
{
    this->quickMenuActive = value;

    if (this->selectedTile != 0 && !this->quickMenuActive)
    {
        if (this->selectedTile == 1)
        {
            WeaponsSystem::changeWeapon(this->registry, redWeapon);
        }
        else if (this->selectedTile == 2)
        {
            WeaponsSystem::changeWeapon(this->registry, blueWeapon);
        }
    }
}

void GUIManager::initializeQuickMenu()
{
    TextureManager::getInstance().loadTexture("ACTIVE_TILE", ASSETS_PATH + std::string("tileActive.png"));
    TextureManager::getInstance().loadTexture("INACTIVE_TILE", ASSETS_PATH + std::string("tileInactive.png"));

    //TODO: this is a temporary solution for loading bullet ico textures
    TextureManager::getInstance().loadTexture("RED_BULLET", ASSETS_PATH + std::string("red_bullet.png"));
    TextureManager::getInstance().loadTexture("BLUE_BULLET", ASSETS_PATH + std::string("blue_bullet.png"));

    float centerX = this->window.getSize().x / 2.f;
    float centerY = this->window.getSize().y / 2.f;
    float radius = 360.f;
    float angleIncrement = 45.f;

    this->quickMenuTiles.resize(8);
    this->quickMenuIcons.resize(8);
    for (int i = 0; i < 8; i++)
    {
        this->quickMenuTiles[i].setTexture(TextureManager::getInstance().getTexture("INACTIVE_TILE"));
        this->quickMenuTiles[i].setOrigin(this->quickMenuTiles[i].getGlobalBounds().width / 2.f, this->quickMenuTiles[i].getGlobalBounds().height / 2.f);
        this->quickMenuTiles[i].setPosition(centerX + std::sin((i * angleIncrement) * M_PI / 180.f) * radius, centerY - std::cos((i * angleIncrement) * M_PI / 180.f) * radius);
        this->quickMenuTiles[i].setRotation(i * angleIncrement);

        //TODO: this is a temporary solution for loading bullet ico textures
        if (i == 0)
        {
            this->quickMenuIcons[i].setTexture(TextureManager::getInstance().getTexture("RED_BULLET"));
        }
        else if (i == 1)
        {
            this->quickMenuIcons[i].setTexture(TextureManager::getInstance().getTexture("BLUE_BULLET"));
        }

        this->quickMenuIcons[i].setOrigin(this->quickMenuIcons[i].getGlobalBounds().width / 2.f, this->quickMenuIcons[i].getGlobalBounds().height / 2.f);
        this->quickMenuIcons[i].setPosition(this->quickMenuTiles[i].getPosition());
    }
}

void GUIManager::initializeShader()
{
    this->shader.loadFromFile(ASSETS_PATH + std::string("shader.frag"), sf::Shader::Fragment);
    this->shaderTexture.create(this->window.getSize().x, this->window.getSize().y);
    this->shaderSprite.setTexture(this->shaderTexture);
}