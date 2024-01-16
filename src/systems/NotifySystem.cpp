#include "NotifySystem.h"

void NotifySystem::notify(const std::string& message, float displayTime)
{
    sf::Text text(message, FontManager::getInstance().getFont("font"), 24); //TODO: set font size
    text.setPosition(20.f, 1080.f);

    if (!notifications.empty())
        for (auto& notification : notifications)
            notification.text.move(0.f, notification.text.getGlobalBounds().height + 20.f); //TODO: set margin

    notifications.push_back({ text });
}

void NotifySystem::update(sf::Time deltaTime)
{
    for (auto& notification : notifications)
    {
        notification.displayTime -= deltaTime.asSeconds();

        if (notification.displayTime < 1.f)
            notification.text.setFillColor(sf::Color(255, 255, 255, 255 * notification.displayTime));
    }

    notifications.remove_if([](const Notification& notification)
    {
        return notification.displayTime <= 0.f;
    });
}

void NotifySystem::draw(sf::RenderWindow& window)
{
    for (const auto& notification : notifications)
            window.draw(notification.text);
}