#include "pch.h"
#include "ConfigManager.hpp"

ConfigManager& ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

void ConfigManager::loadConfig()
{
    std::ifstream file(CONFIG_PATH + std::string("gameConfig.json"));
    nlohmann::json config;

    if (file.is_open())
    {
        file >> config;
        file.close();
    }

    this->SCREEN_WIDTH = config["resolution"]["width"];
    this->SCREEN_HEIGHT = config["resolution"]["height"];
    this->FRAME_RATE_LIMIT = config["frameRateLimit"];
    this->WINDOW_STYLE = config["windowMode"].get<std::string>();
}

float ConfigManager::getScale()
{
    constexpr unsigned int k4KWidth = 3840;
    constexpr unsigned int k4KHeight = 2160;
    constexpr float k4KScale = 1.0f;

    float scaleX = static_cast<float>(SCREEN_WIDTH) / k4KWidth;
    float scaleY = static_cast<float>(SCREEN_HEIGHT) / k4KHeight;

    return std::min(scaleX, scaleY) / k4KScale;
}