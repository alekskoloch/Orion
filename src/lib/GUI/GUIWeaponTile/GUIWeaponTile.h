#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include "ConfigManager.hpp"
#include "TextureManager.h"
#include "FontManager.h"

#include "MoneySystem.h"
#include "CooldownSystem.h"
#include "SkillSystem.h"

#include "weapon.h"
#include "shield.h"
#include "moneyInventory.h"
#include "player.h"

#include "GraphicsOperations.h"

class GUIWeaponTile : public sf::Drawable
{
public:
    GUIWeaponTile(sf::RenderWindow& window, entt::registry& registry);

    void setWeaponTexture(const std::string& textureName);

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    sf::CircleShape circle;
    sf::CircleShape loadingCircle;
    sf::Texture weaponTexture;
    sf::Sprite weaponIcon;

    bool specialShotAvailable = false;
};

//TODO: Refactor this to one tile class
class GUIShieldTile : public sf::Drawable
{
public:
    GUIShieldTile(sf::RenderWindow& window, entt::registry& registry);

    void setShieldTexture(const std::string& textureName);

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    sf::CircleShape circle;
    sf::CircleShape shiedlStateCircle;
    float shieldDuration;
    sf::Texture shieldTexture;
    sf::Sprite shieldIcon;
};

//TODO: Refactor this to one tile class
class GUIMoneyBar : public sf::Drawable
{
public:
    GUIMoneyBar(sf::RenderWindow& window, entt::registry& registry);

    void setString(const std::string& string);

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::RenderWindow& window;
    entt::registry& registry;

    sf::RectangleShape bar;
    sf::Text moneyText;
};