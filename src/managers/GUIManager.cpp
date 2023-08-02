#include "GUIManager.h"

#include <cmath>

#include "../managers/TextureManager.h"

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

GUIManager::GUIManager(sf::RenderWindow& window) : window(window)
{
    this->initializeQuickMenu();
}

void GUIManager::update()
{
    if (this->quickMenuActive)
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(this->window);

        for (auto& tile : this->quickMenuTiles)
        {
            if (isMouseOverSprite(tile, mousePosition))
            {
                tile.setTexture(TextureManager::getInstance().getTexture("ACTIVE_TILE"));
            }
            else
            {
                tile.setTexture(TextureManager::getInstance().getTexture("INACTIVE_TILE"));
            }
        }
    }
}

void GUIManager::draw()
{
    if (this->quickMenuActive)
    {
        for (auto& tile : this->quickMenuTiles)
        {
            this->window.draw(tile);
        }
    }
}

void GUIManager::toggleQuickMenu(bool value)
{
    this->quickMenuActive = value;
}

void GUIManager::initializeQuickMenu()
{
    TextureManager::getInstance().loadTexture("ACTIVE_TILE", ASSETS_PATH + std::string("tileActive.png"));
    TextureManager::getInstance().loadTexture("INACTIVE_TILE", ASSETS_PATH + std::string("tileInactive.png"));

    float centerX = this->window.getSize().x / 2.f;
    float centerY = this->window.getSize().y / 2.f;
    float radius = 360.f;
    float angleIncrement = 45.f;

    this->quickMenuTiles.resize(8);
    for (int i = 0; i < 8; i++)
    {
        this->quickMenuTiles[i].setTexture(TextureManager::getInstance().getTexture("INACTIVE_TILE"));
        this->quickMenuTiles[i].setOrigin(this->quickMenuTiles[i].getGlobalBounds().width / 2.f, this->quickMenuTiles[i].getGlobalBounds().height / 2.f);
        this->quickMenuTiles[i].setPosition(centerX + std::sin((i * angleIncrement) * M_PI / 180.f) * radius, centerY - std::cos((i * angleIncrement) * M_PI / 180.f) * radius);
        this->quickMenuTiles[i].setRotation(i * angleIncrement);
    }
}