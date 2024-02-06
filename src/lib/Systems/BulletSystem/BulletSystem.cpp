#include "BulletSystem.h"

void BulletSystem::updateShurikenBullet(entt::registry& registry, sf::Time deltaTime)
{
    auto shurikenBulletsView = registry.view<ShurikenBullet, Position>();

    for (auto shurikenBulletEntity : shurikenBulletsView)
    {
        auto& shurikenBulletData = shurikenBulletsView.get<ShurikenBullet>(shurikenBulletEntity);
        auto& position = shurikenBulletsView.get<Position>(shurikenBulletEntity);
        
        int multiplier = shurikenBulletData.right ? 1 : -1;

        float x1 = shurikenBulletData.enemyPositionWhenShot.x;
        float y1 = shurikenBulletData.enemyPositionWhenShot.y;

        float x2 = shurikenBulletData.playerPositionWhenShot.x;
        float y2 = shurikenBulletData.playerPositionWhenShot.y;

        float distanceBetweenPoints = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        float radius = distanceBetweenPoints / 2.f;

        float bulletX = ((x1 + x2) / 2) + std::cos((shurikenBulletData.currentTime * multiplier) * (shurikenBulletData.speed/distanceBetweenPoints) + std::atan2(y2 - ((y1 + y2) / 2), x2 - (x1 + x2) / 2)) * radius;
        float bulletY = ((y1 + y2) / 2) + std::sin((shurikenBulletData.currentTime * multiplier) * (shurikenBulletData.speed/distanceBetweenPoints) + std::atan2(y2 - ((y1 + y2) / 2), x2 - (x1 + x2) / 2)) * radius;

        position.position = {bulletX, bulletY};

        shurikenBulletData.currentTime += deltaTime.asSeconds();

        if ((shurikenBulletData.currentTime * shurikenBulletData.speed) / distanceBetweenPoints > 6.25)
            registry.destroy(shurikenBulletEntity);
    }
}