#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class GUIManager
{
public:
    GUIManager(sf::RenderWindow& window, entt::registry& registry);

    void update();
    void draw();

    void toggleQuickMenu(bool value);
private:
    entt::registry& registry;
    sf::RenderWindow& window;

    bool quickMenuActive = false;
    int selectedTile = 0;
    void initializeQuickMenu();
    std::vector<sf::Sprite> quickMenuTiles;
    std::vector<sf::Sprite> quickMenuIcons;

//TODO: move shader
    sf::Shader shader;
    sf::Texture shaderTexture;
    sf::Sprite shaderSprite;
    void initializeShader();
};