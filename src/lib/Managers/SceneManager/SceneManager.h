#pragma once

enum class Scene
{
    MainMenu,
    Settings,
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