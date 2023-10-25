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
}

void SoundManager::playSound(const std::string& soundName)
{
    if(soundBuffers.find(soundName) == soundBuffers.end())
        loadSound(soundName, ASSETS_PATH + std::string("sounds/") + soundName + ".wav");
    sounds[soundName].setBuffer(soundBuffers.at(soundName));
    sounds[soundName].play();
}

void SoundManager::playLoopedSound(const std::string& soundName)
{
    if(soundBuffers.find(soundName) == soundBuffers.end())
        loadSound(soundName, ASSETS_PATH + std::string("sounds/") + soundName + ".wav");
    sounds[soundName].setBuffer(soundBuffers.at(soundName));
    sounds[soundName].setLoop(true);
    sounds[soundName].play();
}

void SoundManager::stopLoopedSound(const std::string& soundName)
{
    if(soundBuffers.find(soundName) == soundBuffers.end())
        return;
    sounds[soundName].stop();
}

bool SoundManager::isLoopedSoundPlaying(const std::string& soundName)
{
    if(soundBuffers.find(soundName) == soundBuffers.end())
        return false;
    return sounds[soundName].getStatus() == sf::Sound::Playing;
}

void SoundManager::loadMusic(const std::string& musicName, const std::string& musicPath)
{
    std::unique_ptr<sf::Music> music = std::make_unique<sf::Music>();
    if (!music->openFromFile(musicPath))
        throw std::runtime_error("Failed to load music: " + musicPath);
    this->music[musicName] = std::move(music);
}

void SoundManager::playMusic(const std::string& musicName)
{
    if (this->music.find(musicName) == this->music.end())
        loadMusic(musicName, ASSETS_PATH + std::string("music/") + musicName + ".wav");
    this->music[musicName]->play();
}

void SoundManager::setLoop(const std::string& soundName, bool loop)
{
    if (music.find(soundName) != music.end())
        music[soundName]->setLoop(loop);
    else
        throw std::runtime_error("Sound not found: " + soundName);
}