#include "TextureManager.h"

TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

void TextureManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
    if(textures.find(textureName) != textures.end())
        return;

    sf::Texture texture;
    if(!texture.loadFromFile(texturePath))
        throw std::runtime_error("Failed to load texture: " + texturePath);
    textures[textureName] = std::move(texture);
}

sf::Texture& TextureManager::getTexture(const std::string& textureName)
{
    if(textures.find(textureName) == textures.end())
        loadTexture(textureName, ASSETS_PATH + textureName + ".png");

    return textures.at(textureName);
}