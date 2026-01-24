#include "SoundManager.h"
#include "pch.h"


SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}

void SoundManager::loadSound( const std::string& soundName, const std::string& soundPath )
{
    if ( soundBuffers.find( soundName ) != soundBuffers.end() )
        return;

    sf::SoundBuffer soundBuffer;

    if ( !soundBuffer.loadFromFile( soundPath ) )
        throw std::runtime_error( "Failed to load sound: " + soundPath );

    soundBuffers[ soundName ] = std::move( soundBuffer );
}

void SoundManager::playSound( const std::string& soundName )
{
    if ( soundBuffers.find( soundName ) == soundBuffers.end() )
        return;

    if ( sounds.find( soundName ) == sounds.end() )
    {
        sounds.emplace( soundName, sf::Sound( soundBuffers.at( soundName ) ) );
    }

    sounds.at( soundName ).play();
}

void SoundManager::playLoopedSound( const std::string& soundName )
{
    if ( soundBuffers.find( soundName ) == soundBuffers.end() )
        loadSound( soundName, ASSETS_PATH + std::string( "sounds/" ) + soundName + ".wav" );

    if ( sounds.find( soundName ) == sounds.end() )
    {
        sounds.emplace( soundName, sf::Sound( soundBuffers.at( soundName ) ) );
    }

    auto& sound = sounds.at( soundName );
    sound.setLooping( true );
    sound.play();
}

void SoundManager::stopLoopedSound( const std::string& soundName )
{
    auto it = sounds.find( soundName );
    if ( it != sounds.end() )
    {
        it->second.stop();
    }
}

bool SoundManager::isLoopedSoundPlaying( const std::string& soundName )
{
    auto it = sounds.find( soundName );
    if ( it == sounds.end() )
        return false;
    return it->second.getStatus() == sf::Sound::Status::Playing;
}

void SoundManager::loadMusic( const std::string& musicName, const std::string& musicPath )
{
    auto musicPtr = std::make_unique< sf::Music >();
    if ( !musicPtr->openFromFile( musicPath ) )
        throw std::runtime_error( "Failed to load music: " + musicPath );
    this->music[ musicName ] = std::move( musicPtr );
}

void SoundManager::playMusic( const std::string& musicName )
{
    if ( this->music.find( musicName ) == this->music.end() )
        loadMusic( musicName, ASSETS_PATH + std::string( "music/" ) + musicName + ".wav" );
    this->music.at( musicName )->play();
}

void SoundManager::setLoop( const std::string& soundName, bool loop )
{
    if ( music.find( soundName ) != music.end() )
        music.at( soundName )->setLooping( loop );
    else if ( sounds.find( soundName ) != sounds.end() )
        sounds.at( soundName ).setLooping( loop );
    else
        throw std::runtime_error( "Sound/Music not found: " + soundName );
}