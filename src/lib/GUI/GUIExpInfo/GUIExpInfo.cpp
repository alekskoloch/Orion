#include "pch.h"
#include "GUIExpInfo.h"

GUIExpInfo::GUIExpInfo(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry)
{
    backgroundBar.setFillColor(sf::Color::Black);
    backgroundBar.setSize(sf::Vector2f(
        220 * ConfigManager::getInstance().getScale(),
        20 * ConfigManager::getInstance().getScale()
    ));
    backgroundBar.setOutlineThickness(4);
    backgroundBar.setOutlineColor(sf::Color::White);
    backgroundBar.setPosition(
        180 * ConfigManager::getInstance().getScale(),
        50 * ConfigManager::getInstance().getScale()
    );

    expBar.setFillColor(sf::Color::Yellow);
    expBar.setSize(sf::Vector2f(0, 20 * ConfigManager::getInstance().getScale()));
    expBar.setPosition(
        180 * ConfigManager::getInstance().getScale(),
        50 * ConfigManager::getInstance().getScale()
    );

    skillPointsAvailable.setFillColor(sf::Color::Yellow);
    skillPointsAvailable.setRadius(20 * ConfigManager::getInstance().getScale());
    skillPointsAvailable.setPosition(
        160 * ConfigManager::getInstance().getScale(),
        50 * ConfigManager::getInstance().getScale()
    );
}

void GUIExpInfo::update()
{
    auto playerEntity = this->registry.view<Player>()[0];
    auto& playerExp = this->registry.get<Experience>(playerEntity).experience;
    auto& playerSkillPoints = this->registry.get<Experience>(playerEntity).skillPoints;
    auto& playerRequiredExp = this->registry.get<Experience>(playerEntity).experienceToNextLevel;

    float expBarWidth = (220.f * ConfigManager::getInstance().getScale()) * (playerExp / (float)playerRequiredExp);
    expBar.setSize(sf::Vector2f(expBarWidth, 20 * ConfigManager::getInstance().getScale()));

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