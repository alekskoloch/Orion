#pragma once

struct ShurikenBullet
{
    sf::Vector2f enemyPositionWhenShot;
    sf::Vector2f playerPositionWhenShot;
    bool right;
    float speed;
    float currentTime = 0.f;
};