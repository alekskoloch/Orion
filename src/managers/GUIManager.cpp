#include "GUIManager.h"

#include <cmath>

#include "../managers/TextureManager.h"

GUIManager::GUIManager(sf::RenderWindow& window) : window(window)
{
    this->initializeQuickMenu();
}

void GUIManager::draw()
{
    for (auto& tile : this->quickMenuTiles)
    {
        this->window.draw(tile);
    }
}

void GUIManager::initializeQuickMenu()
{
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