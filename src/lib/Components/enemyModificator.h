#pragma once

#include <vector>

enum class Modificator
{
    None,
    Immortal,
    TutorialGuard,
    AllySummon,
};

struct EnemyModificator
{
    std::vector<Modificator> modificators;
};
