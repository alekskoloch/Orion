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

    //TODO: This should be handled by GameState?
    bool gameStarted = false;
public:
    static SceneManager& getInstance();

    SceneManager(SceneManager const&) = delete;
    void operator=(SceneManager const&) = delete;

    void setCurrentScene(Scene scene);
    Scene getCurrentScene();

    bool isGameStarted();
    void setGameStarted(bool gameStarted);
};