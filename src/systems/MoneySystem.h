#pragma once

#include "../pch.h"

class MoneySystem
{
public:
    static void addMoney(entt::registry& registry, int amount);
    static unsigned int getMoney(entt::registry& registry);
};