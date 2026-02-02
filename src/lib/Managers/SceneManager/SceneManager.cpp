#include "SceneManager.h"
#include "pch.h"

auto SceneManager::getInstance() -> SceneManager&
{
    static SceneManager instance;
    return instance;
}

void SceneManager::setCurrentScene( Scene scene )
{ 
    this->currentScene = scene;
}

auto SceneManager::getCurrentScene() -> Scene
{
    return this->currentScene;
}

auto SceneManager::isGameStarted() const -> bool
{
    return this->gameStarted;
}

void SceneManager::setGameStarted( bool gameStarted )
{
    this->gameStarted = gameStarted;
}