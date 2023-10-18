#include "InfoSystem.h"

#include "../managers/FontManager.h"

#include "../components/components.h"

void InfoSystem::update(entt::registry& registry, sf::Time deltaTime)
{
    auto infoView = registry.view<Info>();

    for (auto info : infoView)
    {
        auto& infoComponent = infoView.get<Info>(info);

        if (infoComponent.displayTime == 1.f)
        {
            infoComponent.text.setFont(FontManager::getInstance().getFont("font"));
            infoComponent.text.setPosition(infoComponent.position);
            infoComponent.text.setFillColor(infoComponent.color);
            infoComponent.text.setCharacterSize(20);
            infoComponent.text.setString(infoComponent.info);
        }

        infoComponent.displayTime -= deltaTime.asSeconds();

        if (infoComponent.displayTime <= 0.f)
        {
            registry.destroy(info);
        }
    }
}

void InfoSystem::draw(entt::registry& registry, sf::RenderWindow& window)
{
    auto infoView = registry.view<Info>();

    for (auto info : infoView)
    {
        auto& infoComponent = infoView.get<Info>(info);

        window.draw(infoComponent.text);
    }
}
