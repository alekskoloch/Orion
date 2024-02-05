#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

//TODO: Refactor code to use this class
class FontManager
{
private:
    FontManager() = default;
    std::unordered_map<std::string, sf::Font> fonts;
public:
    static FontManager& getInstance();

    FontManager(FontManager const&) = delete;
    void operator=(FontManager const&) = delete;

    void loadFont(const std::string& fontName);
    sf::Font& getFont(const std::string& fontName);
};