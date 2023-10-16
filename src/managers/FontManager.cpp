#include "FontManager.h"

FontManager& FontManager::getInstance()
{
    static FontManager instance;
    return instance;
}

void FontManager::loadFont(const std::string& fontName)
{
    if (fonts.find(fontName) != fonts.end())
        return;
    
    sf::Font font;
    if (!font.loadFromFile(ASSETS_PATH + std::string("fonts/") + fontName + ".ttf"))
        throw std::runtime_error("Failed to load font: " + fontName);

    fonts[fontName] = std::move(font);
}

sf::Font& FontManager::getFont(const std::string& fontName)
{
    if(fonts.find(fontName) == fonts.end())
        loadFont(fontName);

    return fonts.at(fontName);
}