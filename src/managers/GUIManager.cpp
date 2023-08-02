#include "GUIManager.h"

#include "../managers/TextureManager.h"

#include <iostream>

GUIManager::GUIManager(sf::RenderWindow& window) : window(window)
{
    this->initializeQuickMenu();
}

void GUIManager::draw()
{
    this->window.draw(this->quickMenuTileInactive);
}

void GUIManager::initializeQuickMenu()
{
    TextureManager::getInstance().loadTexture("ACTIVE_TILE", ASSETS_PATH + std::string("tileActive.png"));
    TextureManager::getInstance().loadTexture("INACTIVE_TILE", ASSETS_PATH + std::string("tileInactive.png"));

    this->quickMenuTileActive.setTexture(TextureManager::getInstance().getTexture("ACTIVE_TILE"));
    this->quickMenuTileInactive.setTexture(TextureManager::getInstance().getTexture("INACTIVE_TILE"));
}