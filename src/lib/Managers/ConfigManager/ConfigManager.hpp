#pragma once

#include <string>

#include "nlohmann/json.hpp"

class ConfigManager
{
private:
    ConfigManager() = default;
    unsigned int SCREEN_WIDTH;
    unsigned int SCREEN_HEIGHT;
    unsigned int FRAME_RATE_LIMIT;
public:
    static ConfigManager& getInstance();

    ConfigManager(ConfigManager const&) = delete;
    void operator=(ConfigManager const&) = delete;

    void loadConfig();

    unsigned int getScreenWidth() const { return SCREEN_WIDTH; }
    unsigned int getScreenHeight() const { return SCREEN_HEIGHT; }
    unsigned int getFrameRateLimit() const { return FRAME_RATE_LIMIT; }
};