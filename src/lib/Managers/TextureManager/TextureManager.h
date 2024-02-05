#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class TextureManager
{
private:
    TextureManager() = default;
    std::unordered_map<std::string, sf::Texture> textures;
public:
    static TextureManager& getInstance();

    TextureManager(TextureManager const&) = delete;
    void operator=(TextureManager const&) = delete;

    void loadTexture(const std::string& textureName, const std::string& texturePath);
    sf::Texture& getTexture(const std::string& textureName);
};