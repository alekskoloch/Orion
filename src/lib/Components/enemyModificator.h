#pragma once

#include <vector>

enum class Modificator
{
    None,
    TutorialGuard,
    AllySummon,
};

struct EnemyModificator
{
    std::vector<Modificator> modificators;
};
