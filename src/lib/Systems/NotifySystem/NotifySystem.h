#pragma once

#include <SFML/Graphics.hpp>
#include "GUIDialogBox.h"
#include "CooldownSystem.h"
#include "FontManager.h"

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
    static void notifyDialogBox(sf::RenderWindow& window, const std::string& message, const std::string& buttonMessage, std::function<void()> callback);
    static bool isDialogBoxActive();
    static void update(sf::Time deltaTime);
    static void draw(sf::RenderWindow& window);

private:
    static inline std::list<Notification> notifications;
    static inline std::queue<Notification> bigInfoQueue;
    static inline std::unique_ptr<GUIDialogBox> dialogBox = nullptr;
    
};