#include "pch.h"
#include "GUIEnergyBar.h"

GUIEnergyBar::GUIEnergyBar(sf::RenderWindow& window, entt::registry& registry)
    : window(window), registry(registry)
{
    this->initializeEnergyBar();
}

void GUIEnergyBar::update(sf::Time deltaTime)
{
    EventManager::getInstance().subscribe(EventManager::Event::NotEnoughEnergy, [this]()
    {
        // this->energyBarSprite.setFillColor(sf::Color::Red);
        this->isNotEnoughEnergy = true;
    });

    if (this->isNotEnoughEnergy)
    {
        if (this->blinkTimer <= 0.f)
        {
            this->blink = !this->blink;
            this->blinkTimer = 0.2f;
        }

        if (this->blink)
            this->energyBarBackgroundSprite.setOutlineColor(sf::Color::Red);
        else
            this->energyBarBackgroundSprite.setOutlineColor(sf::Color::White);

        this->notEnoughEnergyTimer -= deltaTime.asSeconds();
        this->blinkTimer -= deltaTime.asSeconds();

        if (this->notEnoughEnergyTimer <= 0.f)
        {
            this->isNotEnoughEnergy = false;
            this->notEnoughEnergyTimer = 2.f;
        }
    }
    else
    {
        this->energyBarBackgroundSprite.setOutlineColor(sf::Color::White);
    }

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