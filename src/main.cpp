#include "Game.h"
#include "ConfigManager.hpp"

int main()
{
    ConfigManager::getInstance().loadOrCreateConfig();

    Game game;
    game.run();

    return EXIT_SUCCESS;
}