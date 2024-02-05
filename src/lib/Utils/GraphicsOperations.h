#pragma once

#include <SFML/Graphics.hpp>

inline sf::Sprite CreateSprite(const std::string& textureName)
{
    sf::Sprite sprite(TextureManager::getInstance().getTexture(textureName));
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    return sprite;
}