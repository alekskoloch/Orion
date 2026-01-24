#pragma once

#include <SFML/Graphics.hpp>
#include "TextureManager.h"

inline sf::Sprite CreateSprite(const std::string& textureName)
{
    sf::Sprite sprite(TextureManager::getInstance().getTexture(textureName));
    sprite.setOrigin(sprite.getGlobalBounds().getCenter() );
    return sprite;
}