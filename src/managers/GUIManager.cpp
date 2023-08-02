#include "GUIManager.h"

#include "../managers/TextureManager.h"

GUIManager::GUIManager(sf::RenderWindow& window) : window(window) {}

void GUIManager::draw()
{
    
}

void GUIManager::initializeQuickMenu()
{
    TextureManager::getInstance().loadTexture("ACTIVE_TILE", ASSETS_PATH + std::string("tileActive.png"));
    TextureManager::getInstance().loadTexture("INACTIVE_TILE", ASSETS_PATH + std::string("tileInactive.png"));

    this->quickMenuTileActive.setTexture(TextureManager::getInstance().getTexture("ACTIVE_TILE"));
    this->quickMenuTileInactive.setTexture(TextureManager::getInstance().getTexture("INACTIVE_TILE"));
}