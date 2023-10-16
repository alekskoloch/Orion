#pragma once

#include "../../pch.h"

class GUIWeaponTile : public sf::Drawable
{
public:
    GUIWeaponTile(sf::RenderWindow& window, entt::registry& registry);

    void setWeaponTexture(const std::string& textureName);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    sf::CircleShape circle;
    sf::Texture weaponTexture;
    sf::Sprite weaponIcon;
};

//TODO: Refactor this to one tile class
class GUIShieldTile : public sf::Drawable
{
public:
    GUIShieldTile(sf::RenderWindow& window, entt::registry& registry);

    void setShieldTexture(const std::string& textureName);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    sf::CircleShape circle;
    sf::Texture shieldTexture;
    sf::Sprite shieldIcon;
};

class GUIMoneyBar : public sf::Drawable
{
public:
    GUIMoneyBar(sf::RenderWindow& window, entt::registry& registry);

    void setString(const std::string& string);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    sf::RectangleShape bar;
    sf::Text moneyText;
};