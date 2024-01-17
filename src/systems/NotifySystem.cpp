#include "NotifySystem.h"

void NotifySystem::notify(const Type type, const std::string& message, float displayTime)
{
    sf::Text text;
    switch (type)
    {
    case Type::Info:
        text = sf::Text(message, FontManager::getInstance().getFont("font"), 24); //TODO: set font size
        text.setPosition(20.f, 1080.f);

        if (!notifications.empty())
            for (auto& notification : notifications)
                if (notification.text.getCharacterSize() == 24)
                    notification.text.move(0.f, notification.text.getGlobalBounds().height + 20.f); //TODO: set margin

        notifications.push_back({ text, displayTime });
        break;
    case Type::BigInfo:
        text = sf::Text(message, FontManager::getInstance().getFont("font"), 48); //TODO: set font size
        text.setPosition(3840.f / 2.f - text.getGlobalBounds().width / 2.f, 200.f);
        text.setOutlineColor(sf::Color::Red);
        text.setOutlineThickness(2.f);

        notifications.remove_if([](const Notification& notification)
        {
            return notification.text.getCharacterSize() == 48;
        });

        notifications.push_back({ text, displayTime });

        break;
    default:
        throw std::runtime_error("Unknown notification type");
    }
}

void NotifySystem::notifyDialogBox(sf::RenderWindow& window, const std::string& message, const std::string& buttonMessage, std::function<void()> callback)
{
    if (dialogBox != nullptr)
        return;

    std::vector<std::string> messages = { message };
    dialogBox = std::make_unique<GUIDialogBox>(window, messages, FontManager::getInstance().getFont("font"));
    dialogBox->setType(GUIDialogBoxType::Ok);
    dialogBox->setState(GUIDialogBoxState::Idle);
    dialogBox->setMessage({ message });
}

bool NotifySystem::isDialogBoxActive()
{
    return dialogBox != nullptr;
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

    if (dialogBox != nullptr)
    {
        dialogBox->update();

        if (dialogBox->getState() == GUIDialogBoxState::Hidden)
            dialogBox = nullptr;
        else if (dialogBox->getState() == GUIDialogBoxState::Ok)
        {
            dialogBox->setState(GUIDialogBoxState::Hidden);
        }
    }
}

void NotifySystem::draw(sf::RenderWindow& window)
{
    for (const auto& notification : notifications)
            window.draw(notification.text);

    if (dialogBox != nullptr)
        dialogBox->draw();
}