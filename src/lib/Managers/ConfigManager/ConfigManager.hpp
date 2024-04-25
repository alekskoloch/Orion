#pragma once

#include <string>

#include "nlohmann/json.hpp"
#include <SFML/Graphics.hpp>

class ConfigManager
{
private:
    ConfigManager() = default;
    unsigned int SCREEN_WIDTH;
    unsigned int SCREEN_HEIGHT;
    unsigned int FRAME_RATE_LIMIT;
    std::string WINDOW_STYLE;

public:
    static ConfigManager& getInstance();

    ConfigManager(ConfigManager const&) = delete;
    void operator=(ConfigManager const&) = delete;

    void loadConfig();

    unsigned int getScreenWidth() const { return SCREEN_WIDTH; }
    unsigned int getScreenHeight() const { return SCREEN_HEIGHT; }
    unsigned int getFrameRateLimit() const { return FRAME_RATE_LIMIT; }
    unsigned int getWindowStyle() const { return this->WINDOW_STYLE == "Fullscreen" ? sf::Style::Fullscreen : sf::Style::Default; }

    float getScale();
    bool isTutorialEnabled();
};

