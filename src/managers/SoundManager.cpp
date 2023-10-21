#include "SoundManager.h"

SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}

void SoundManager::loadSound(const std::string& soundName, const std::string& soundPath)
{
    if(soundBuffers.find(soundName) != soundBuffers.end())
        return;

    sf::SoundBuffer soundBuffer;

    if(!soundBuffer.loadFromFile(soundPath))
        throw std::runtime_error("Failed to load sound: " + soundPath);

    soundBuffers[soundName] = std::move(soundBuffer);

    std::cout << "Loaded sound: " << soundName << std::endl;
}