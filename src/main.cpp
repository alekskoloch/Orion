#include "ConfigManager.hpp"
#include "Game.h"

auto main() -> int
{
    ConfigManager::getInstance().loadConfig();

    Game game;
    game.run();

    return EXIT_SUCCESS;
}