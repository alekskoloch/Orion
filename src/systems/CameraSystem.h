#pragma once

#include "../pch.h"

class CameraSystem
{
public:
    static void setDefaultCamera(sf::RenderWindow& window);
    static void setPlayerCamera(entt::registry& registry, sf::RenderWindow& window);
    static sf::View& getView();
private:
    static sf::View view;
};