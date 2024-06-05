#pragma once

#include <string>

struct ShieldSchema
{
    float durability;
    float energyCost;
    float duration;

    float loadTime;

    std::string shieldTextureName;
    std::string shieldIconTextureName;
};

static ShieldSchema basicShield =
{
    1.f,
    50.f,
    15.f,
    2.f,
    "shield",
    "basic_shield_ico"
};

static ShieldSchema advancedShield =
{
    1.f,
    100.f,
    30.f,
    4.f,
    "shield_advanced",
    "advanced_shield_ico"
};