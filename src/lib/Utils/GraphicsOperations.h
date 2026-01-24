#pragma once

#include "TextureManager.h"
#include <SFML/Graphics.hpp>

inline sf::Sprite CreateSprite( const std::string& textureName )
{
    //TextureManager::getInstance().loadTexture(

    const sf::Texture& texture = TextureManager::getInstance().getTexture( textureName );

    sf::Sprite sprite( texture );

    sf::FloatRect bounds = sprite.getGlobalBounds();
    sprite.setOrigin( { bounds.size.x / 2.f, bounds.size.y / 2.f } );

    return sprite;
}