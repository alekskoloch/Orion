#pragma once

#include <vector>

enum class Modificator
{
    AllySummon
};

struct EnemyModificator
{
    std::vector<Modificator> modificators;
};
