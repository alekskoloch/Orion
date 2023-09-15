#pragma once

#include "../pch.h"

struct SkillSchema
{
    sf::Vector2f position;
    std::string name;
    std::string description;
    std::string textureName;
    std::string hoverTextureName;
    std::string activeTextureName;
    std::function<void(entt::registry& registry)> callback;
};