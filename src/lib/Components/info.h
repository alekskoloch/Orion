#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "FontManager.h"

struct Info 
{
    std::string info;
    sf::Vector2f position;
    sf::Color color;
    sf::Text text{ FontManager::getInstance().getFont("font") };
    float displayTime = 1.f;
};