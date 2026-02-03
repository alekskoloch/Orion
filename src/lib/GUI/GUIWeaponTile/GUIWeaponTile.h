#pragma once

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

class Window;

class GUIWeaponTile : public sf::Drawable
{
public:
    explicit GUIWeaponTile( entt::registry& registry );

    void setWeaponTexture(const std::string& textureName);

    void update();
    void draw( sf::RenderTarget& target, sf::RenderStates states ) const override;
    void draw( Window& window );

    void clear();
private:
    entt::registry& registry;

    void initialize();

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
    explicit GUIShieldTile( entt::registry& registry );

    void setShieldTexture(const std::string& textureName);

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void draw( Window& window );

    void clear();
private:
    entt::registry& registry;

    void initialize();

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
    explicit GUIMoneyBar( entt::registry& registry );

    void setString(const std::string& string);

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void draw( Window& window );

private:
    entt::registry& registry;

    sf::RectangleShape bar;
    sf::Text moneyText;
};