#include "pch.h"
#include "GUIWeaponTile.h"

GUIWeaponTile::GUIWeaponTile(sf::RenderWindow& window, entt::registry& registry) : window(window), registry(registry)
{
    this->circle.setRadius(200.f);
    this->circle.setFillColor(sf::Color(0, 0, 0));
    this->circle.setOutlineColor(sf::Color::White);
    this->circle.setOutlineThickness(4.f);
    this->circle.setPointCount(50);
    this->circle.setOrigin(this->circle.getGlobalBounds().width / 2.f, this->circle.getGlobalBounds().height / 2.f);

    this->loadingCircle.setRadius(100.f);
    this->loadingCircle.setFillColor(sf::Color(255,0,0,50));
    this->circle.setPointCount(50);
    this->loadingCircle.setOrigin(this->loadingCircle.getGlobalBounds().width / 2.f, this->loadingCircle.getGlobalBounds().height / 2.f);
    this->loadingCircle.setPosition(0, 0);

    this->setWeaponTexture("red_weapon");
}

void GUIWeaponTile::setWeaponTexture(const std::string& textureName)
{
    this->weaponIcon = CreateSprite(textureName);
    float iconWidth = this->weaponIcon.getGlobalBounds().width;
    this->weaponIcon.setPosition(120 - iconWidth / 2, 120 - iconWidth / 2);
}

void GUIWeaponTile::update()
{
    //TODO: refactor this
    auto playerWeaponComponent = this->registry.get<Weapon>(this->registry.view<Player>()[0]);
    if (playerWeaponComponent.type == WeaponType::SingleShot)
    {
        if (SkillSystem::isSkillEnabled(this->registry, SkillType::SingleShotWeaponSpecialShot))
            this->specialShotAvailable = true;
        else
            this->specialShotAvailable = false;
    }
    else if (playerWeaponComponent.type == WeaponType::TrippleShot)
    {
        if (SkillSystem::isSkillEnabled(this->registry, SkillType::TripleShotWeaponSpecialShot))
            this->specialShotAvailable = true;
        else
            this->specialShotAvailable = false;
    }

    //TODO: make update loading circle function
    if (this->specialShotAvailable)
    {
        auto playerEntity = this->registry.view<Player>()[0];

        if (CooldownSystem::getCooldown(this->registry, playerEntity, "specialShot") != -1.f)
        {
            float cooldown = CooldownSystem::getCooldown(this->registry, playerEntity, "specialShot");
            //TODO: get this from weapon
            float maxCooldown = 5.f;
            this->loadingCircle.setRadius(200.f * (1 - cooldown / maxCooldown));

            this->loadingCircle.setOrigin(this->loadingCircle.getGlobalBounds().width / 2.f, this->loadingCircle.getGlobalBounds().height / 2.f);
            this->loadingCircle.setPosition(0, 0);
        }
    }
}

void GUIWeaponTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->circle, states);
    if (this->specialShotAvailable)
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

    this->shiedlStateCircle.setRadius(0.f);
    this->shiedlStateCircle.setFillColor(sf::Color(0,0,255,100));
    this->circle.setPointCount(50);
    this->shiedlStateCircle.setOrigin(this->shiedlStateCircle.getGlobalBounds().width / 2.f, this->shiedlStateCircle.getGlobalBounds().height / 2.f);
    this->shiedlStateCircle.setPosition(0, 200);

    this->setShieldTexture("basic_shield_ico");
}

void GUIShieldTile::setShieldTexture(const std::string& textureName)
{
    this->shieldIcon = CreateSprite(textureName);
    this->shieldIcon.setScale(0.8f, 0.8f);
    this->shieldIcon.setPosition(this->circle.getPosition().x + 55, this->circle.getPosition().y + 55);
}

void GUIShieldTile::update()
{
    //TODO: make update shield state circle function
    auto playerEntity = this->registry.view<Player>()[0];

    //TODO: find better way to do this
    if (!this->registry.get<Shield>(playerEntity).active && this->shieldDuration != this->registry.get<Shield>(playerEntity).duration)
    {
        this->shieldDuration = this->registry.get<Shield>(playerEntity).duration;
    }
    
    if (CooldownSystem::getCooldown(this->registry, playerEntity, "shieldCooldown") != -1.f)
    {
        float cooldown = CooldownSystem::getCooldown(this->registry, playerEntity, "shieldCooldown");

        this->shiedlStateCircle.setRadius(150.f * (cooldown / shieldDuration));

        this->shiedlStateCircle.setOrigin(this->shiedlStateCircle.getGlobalBounds().width / 2.f, this->shiedlStateCircle.getGlobalBounds().height / 2.f);
    }
}

void GUIShieldTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->circle, states);
    target.draw(this->shiedlStateCircle, states);
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