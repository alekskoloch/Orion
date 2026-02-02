#pragma once

enum class Scene : std::uint8_t
{
    MainMenu,
    Settings,
    Game,
    SkillTree,
    GameOver,
    QuitGame
};

class SceneManager
{
private:
    SceneManager() = default;
    Scene currentScene{ Scene::MainMenu };

    //TODO: This should be handled by GameState?
    bool gameStarted = false;
public:
    static SceneManager& getInstance();

    SceneManager(SceneManager const&) = delete;
    void operator=(SceneManager const&) = delete;

    void setCurrentScene(Scene scene);
    auto getCurrentScene() -> Scene;

    [[nodiscard]] auto isGameStarted() const -> bool;
    void setGameStarted(bool gameStarted);
};