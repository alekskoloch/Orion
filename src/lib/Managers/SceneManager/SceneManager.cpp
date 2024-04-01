#include "pch.h"
#include "SceneManager.h"

SceneManager& SceneManager::getInstance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::setCurrentScene(Scene scene)
{
    this->currentScene = scene;
}

Scene SceneManager::getCurrentScene()
{
    return this->currentScene;
}

bool SceneManager::isGameStarted()
{
    return this->gameStarted;
}

void SceneManager::setGameStarted(bool gameStarted)
{
    this->gameStarted = gameStarted;
}