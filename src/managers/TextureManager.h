#pragma once

#include <unordered_map>
#include <string>

#include <SFML/Graphics.hpp>

class TextureManager
{
private:
    std::unordered_map<std::string, sf::Texture> textures;
public:

};