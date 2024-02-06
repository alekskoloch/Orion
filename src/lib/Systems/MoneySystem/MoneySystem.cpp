#include "MoneySystem.h"



void MoneySystem::addMoney(entt::registry& registry, int amount)
{
    auto& money = registry.get<MoneyInventory>(registry.view<Player>()[0]);
    money.money += amount;
}

unsigned int MoneySystem::getMoney(entt::registry& registry)
{
    return registry.get<MoneyInventory>(registry.view<Player>()[0]).money;
}