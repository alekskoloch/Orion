#include "pch.h"
#include "MusicSystem.h"

void MusicSystem::initializeMusic()
{
    //TODO: Reducing the size of a music file
    SoundManager::getInstance().playMusic("music");
    SoundManager::getInstance().setLoop("music", true);
}