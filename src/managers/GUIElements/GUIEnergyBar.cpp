#include "GUIEnergyBar.h"

#include "../../components/components.h"
#include "../../components/tagComponents.h"

GUIEnergyBar::GUIEnergyBar(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry)
{
    this->initializeEnergyBar();
}

void GUIEnergyBar::update()
{
    auto playerEnergyView = this->registry.view<Player, Energy>();
    for (auto player : playerEnergyView)
    {
        auto& playerEnergy = playerEnergyView.get<Energy>(player);

        this->energyBarSprite.setSize(sf::Vector2f((playerEnergy.currentEnergyValue / playerEnergy.maxEnergyValue) * 300, 50));
    }
}

void GUIEnergyBar::draw()
{
    this->window.draw(this->energyBarSprite);
    this->window.draw(this->energyBarBackgroundSprite);
}

void GUIEnergyBar::initializeEnergyBar()
{
    //TODO: Temporary solution
    this->energyBarSprite.setSize(sf::Vector2f(300, 50));
    this->energyBarSprite.setFillColor(sf::Color::Blue);
    this->energyBarSprite.setPosition(3300, 2000);

    this->energyBarBackgroundSprite.setSize(sf::Vector2f(300, 50));
    this->energyBarBackgroundSprite.setFillColor(sf::Color::Transparent);
    this->energyBarBackgroundSprite.setOutlineColor(sf::Color::White);
    this->energyBarBackgroundSprite.setOutlineThickness(-5.0f);
    this->energyBarBackgroundSprite.setPosition(3300, 2000);
}