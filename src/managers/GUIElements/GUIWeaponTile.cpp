#include "GUIWeaponTile.h"

#include "../TextureManager.h"
#include "../FontManager.h"

#include "../../systems/MoneySystem.h"

#include "../../components/components.h"
#include "../../components/tagComponents.h"

#include "../../utils/GraphicsOperations.h"

GUIWeaponTile::GUIWeaponTile(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry)
{
    this->circle.setRadius(200.f);
    this->circle.setFillColor(sf::Color(0, 0, 0));
    this->circle.setOutlineColor(sf::Color::White);
    this->circle.setOutlineThickness(4.f);
    this->circle.setPointCount(50);
    this->circle.setOrigin(this->circle.getGlobalBounds().width / 2.f, this->circle.getGlobalBounds().height / 2.f);

    this->loadingCircle.setRadius(100.f);
    this->loadingCircle.setFillColor(sf::Color(255,0,0,150));
    this->circle.setPointCount(50);
    this->loadingCircle.setOrigin(this->loadingCircle.getGlobalBounds().width / 2.f, this->loadingCircle.getGlobalBounds().height / 2.f);

    this->setWeaponTexture("red_weapon");
}

void GUIWeaponTile::setWeaponTexture(const std::string& textureName)
{
    this->weaponIcon = CreateSprite(textureName);
    float iconWidth = this->weaponIcon.getGlobalBounds().width;
    this->weaponIcon.setPosition(120 - iconWidth / 2, 120 - iconWidth / 2);
}

void GUIWeaponTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->circle, states);
    target.draw(this->loadingCircle, states);
    target.draw(this->weaponIcon, states);
}

GUIShieldTile::GUIShieldTile(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry)
{
    this->circle.setRadius(150.f);
    this->circle.setFillColor(sf::Color(0, 0, 0));
    this->circle.setOutlineColor(sf::Color::White);
    this->circle.setOutlineThickness(4.f);
    this->circle.setPointCount(50);
    this->circle.setOrigin(this->circle.getGlobalBounds().width / 2.f, this->circle.getGlobalBounds().height / 2.f);
    this->circle.setPosition(0, 200);

    this->setShieldTexture("basic_shield_ico");
}

void GUIShieldTile::setShieldTexture(const std::string& textureName)
{
    this->shieldIcon = CreateSprite(textureName);
    this->shieldIcon.setScale(0.8f, 0.8f);
    this->shieldIcon.setPosition(this->circle.getPosition().x + 55, this->circle.getPosition().y + 55);
}

void GUIShieldTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->circle, states);
    target.draw(this->shieldIcon, states);
}

GUIMoneyBar::GUIMoneyBar(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry)
{
    this->bar.setSize(sf::Vector2f(300, 50));
    this->bar.setFillColor(sf::Color(0, 0, 0));
    this->bar.setOutlineColor(sf::Color::White);
    this->bar.setOutlineThickness(4.f);
    this->bar.setPosition(100, 0);

    this->moneyText.setFont(FontManager::getInstance().getFont("font"));
    this->moneyText.setCharacterSize(30);
    this->moneyText.setFillColor(sf::Color::White);
    //TODO: change position
    this->moneyText.setPosition(250, 15);
    this->moneyText.setString("0$");
}

void GUIMoneyBar::setString(const std::string& string)
{
    this->moneyText.setString(string);
}

void GUIMoneyBar::update()
{
    auto& playerMoney = this->registry.get<MoneyInventory>(this->registry.view<Player>()[0]);
    this->moneyText.setString(std::to_string(playerMoney.money) + "$");
}

void GUIMoneyBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->bar, states);
    target.draw(this->moneyText, states);
}