#pragma once

#include "../pch.h"

class SoundManager
{
private:
    SoundManager() = default;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;

public:
    static SoundManager& getInstance();

    SoundManager(const SoundManager&) = delete;
    void operator=(const SoundManager&) = delete;
};