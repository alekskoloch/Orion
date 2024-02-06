#pragma once

#include <entt/entt.hpp>

#include "moneyInventory.h"
#include "player.h"

class MoneySystem
{
public:
    static void addMoney(entt::registry& registry, int amount);
    static unsigned int getMoney(entt::registry& registry);
};