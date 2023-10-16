#include "GUIWeaponTile.h"

#include "../TextureManager.h"

#include "../../utils/GraphicsOperations.h"

GUIWeaponTile::GUIWeaponTile(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry)
{
    this->circle.setRadius(200.f);
    this->circle.setFillColor(sf::Color(225, 225, 225));
    this->circle.setOutlineColor(sf::Color::White);
    this->circle.setOutlineThickness(4.f);
    this->circle.setOrigin(this->circle.getGlobalBounds().width / 2.f, this->circle.getGlobalBounds().height / 2.f);

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
    target.draw(this->weaponIcon, states);
}