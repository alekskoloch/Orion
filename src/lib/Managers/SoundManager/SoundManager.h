#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class SoundManager
{
private:
    SoundManager() = default;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> music;

public:
    static SoundManager& getInstance();

    SoundManager(const SoundManager&) = delete;
    void operator=(const SoundManager&) = delete;

    void loadSound(const std::string& soundName, const std::string& soundPath);
    void playSound(const std::string& soundName);
    void playLoopedSound(const std::string& soundName);
    void stopLoopedSound(const std::string& soundName);
    bool isLoopedSoundPlaying(const std::string& soundName);

    void loadMusic(const std::string& musicName, const std::string& musicPath);
    void playMusic(const std::string& musicName);

    void setLoop(const std::string& soundName, bool loop);
};