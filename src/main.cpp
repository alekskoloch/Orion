#include "Game.h"
#include "ConfigManager.hpp"

int main()
{
    ConfigManager::getInstance().loadConfig();

    Game game;
    game.run();

    return EXIT_SUCCESS;
}