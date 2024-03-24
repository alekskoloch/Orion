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
}