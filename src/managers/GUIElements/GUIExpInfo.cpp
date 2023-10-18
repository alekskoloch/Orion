#include "GUIExpInfo.h"

#include "../../components/components.h"
#include "../../components/tagComponents.h"

GUIExpInfo::GUIExpInfo(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry)
{
    backgroundBar.setFillColor(sf::Color::Black);
    backgroundBar.setSize(sf::Vector2f(200, 20));
    backgroundBar.setOutlineThickness(4);
    backgroundBar.setOutlineColor(sf::Color::White);
    backgroundBar.setPosition(200, 50);

    expBar.setFillColor(sf::Color::Yellow);
    expBar.setSize(sf::Vector2f(0, 20));
    expBar.setPosition(200, 50);
}

void GUIExpInfo::update()
{
    auto playerEntity = this->registry.view<Player>()[0];
    auto& playerExp = this->registry.get<Experience>(playerEntity).experience;
    auto& playerRequiredExp = this->registry.get<Experience>(playerEntity).experienceToNextLevel;

    float expBarWidth = 200.f * (playerExp / (float)playerRequiredExp);
    expBar.setSize(sf::Vector2f(expBarWidth, 20));
}

void GUIExpInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->backgroundBar, states);
    target.draw(this->expBar, states);
}