#pragma once

#include "../pch.h"

#include "../components/components.h"

#include "../managers/FontManager.h"

#include "CooldownSystem.h"

class NotifySystem
{
public:
    struct Notification
    {
        sf::Text text;
        float displayTime = 4.f;
    };

    enum class Type
    {
        Info,
        BigInfo
    };

    static void notify(const Type type, const std::string& message, float displayTime = 4.f);
    static void update(sf::Time deltaTime);
    static void draw(sf::RenderWindow& window);
    
private:
    static inline std::list<Notification> notifications;
};