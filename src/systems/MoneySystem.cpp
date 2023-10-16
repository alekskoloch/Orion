#include "MoneySystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

void MoneySystem::addMoney(entt::registry& registry, int amount)
{
    auto& money = registry.get<MoneyInventory>(registry.view<Player>()[0]);
    money.money += amount;
}