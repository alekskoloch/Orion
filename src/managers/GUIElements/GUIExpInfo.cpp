#include "GUIExpInfo.h"

#include "../../components/components.h"
#include "../../components/tagComponents.h"

GUIExpInfo::GUIExpInfo(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry)
{
    backgroundBar.setFillColor(sf::Color::Black);
    backgroundBar.setSize(sf::Vector2f(220, 20));
    backgroundBar.setOutlineThickness(4);
    backgroundBar.setOutlineColor(sf::Color::White);
    backgroundBar.setPosition(180, 50);

    expBar.setFillColor(sf::Color::Yellow);
    expBar.setSize(sf::Vector2f(0, 20));
    expBar.setPosition(180, 50);

    skillPointsAvailable.setFillColor(sf::Color::Yellow);
    skillPointsAvailable.setRadius(20);
    skillPointsAvailable.setPosition(160, 50);
}

void GUIExpInfo::update()
{
    auto playerEntity = this->registry.view<Player>()[0];
    auto& playerExp = this->registry.get<Experience>(playerEntity).experience;
    auto& playerSkillPoints = this->registry.get<Experience>(playerEntity).skillPoints;
    auto& playerRequiredExp = this->registry.get<Experience>(playerEntity).experienceToNextLevel;

    float expBarWidth = 220.f * (playerExp / (float)playerRequiredExp);
    expBar.setSize(sf::Vector2f(expBarWidth, 20));

    if (playerSkillPoints > 0)
        this->skillPointsAvailableVisible = true;
    else
        this->skillPointsAvailableVisible = false;
}

void GUIExpInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (this->skillPointsAvailableVisible)
        target.draw(this->skillPointsAvailable, states);
    target.draw(this->backgroundBar, states);
    target.draw(this->expBar, states);
}