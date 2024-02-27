#pragma once

#include <vector>

struct GroupFormation
{
    std::vector<std::vector<int>> formation;
    std::vector<std::vector<bool>> seats;

    bool isFormationFull() const
    {
        for (auto& row : seats)
            for (auto seat : row)
                if (!seat)
                    return false;
        return true;
    }
};

struct EnemyGroup
{

};