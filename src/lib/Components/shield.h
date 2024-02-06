#pragma once

#include "ShieldSchema.h"

#include "TextureManager.h"

struct Shield
{
    float durability;
    float energyCost;
    float duration;
    float loadTime;

    std::string shieldTextureName;
    std::string shieldIconTextureName;
    
    float energyUsed = 0.f;

    bool active = false;

    void Set(ShieldSchema shield)
    {
        durability = shield.durability;
        energyCost = shield.energyCost;
        duration = shield.duration;
        loadTime = shield.loadTime;

        energyUsed = 0.f;

        TextureManager::getInstance().loadTexture(shield.shieldTextureName, ASSETS_PATH + shield.shieldTextureName + ".png");
        shieldTextureName = shield.shieldTextureName;
        TextureManager::getInstance().loadTexture(shield.shieldIconTextureName, ASSETS_PATH + shield.shieldIconTextureName + ".png");
        shieldIconTextureName = shield.shieldIconTextureName;
    }
};