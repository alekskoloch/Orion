#pragma once

#include "../pch.h"

enum class Scene
{
    MainMenu,
    Game,
    SkillTree,
    GameOver
};

class SceneManager
{
private:
    SceneManager() = default;
    Scene currentScene;
public:
    static SceneManager& getInstance();

    SceneManager(SceneManager const&) = delete;
    void operator=(SceneManager const&) = delete;

    void setCurrentScene(Scene scene);
    Scene getCurrentScene();
};