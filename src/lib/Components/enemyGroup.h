#pragma once

#include <entt/entt.hpp>
#include <vector>
#include <algorithm>

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

    int getCurrentPositionPriority()
    {
        if (isFormationFull())
            return -1;

        int maxPriority = 0;
        for (const auto& row : formation) {
            auto rowMax = *std::max_element(row.begin(), row.end());
            if (rowMax > maxPriority) {
                maxPriority = rowMax;
            }
        }

        for (int priority = 1; priority <= maxPriority; ++priority) {
            for (size_t i = 0; i < formation.size(); ++i) {
                for (size_t j = 0; j < formation[i].size(); ++j) {
                    if (formation[i][j] == priority && !seats[i][j]) {
                        return priority;
                    }
                }
            }
        }

        return -1; 
    }
};

struct EnemyGroup
{

};