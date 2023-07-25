#include "TextureManager.h"

TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}