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